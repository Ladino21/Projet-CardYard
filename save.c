// === save.c ===
#include <stdio.h>
#include <string.h>
#include "save.h"

int sauvegarder_partie(const char *fichier, Joueur *joueurs, int nb_joueurs, Carte *pioche, int nb_pioche) {
    FILE *f = fopen(fichier, "wb");
    if (!f) return 0;

    fwrite(&nb_joueurs, sizeof(int), 1, f);
    fwrite(&joueurs[0].nb_cartes, sizeof(int), 1, f);
    fwrite(&nb_pioche, sizeof(int), 1, f);
    fwrite(pioche, sizeof(Carte), nb_pioche, f);

    for (int i = 0; i < nb_joueurs; i++) {
        fwrite(joueurs[i].nom, sizeof(char), MAX_NOM, f);
        fwrite(&joueurs[i].nb_cartes, sizeof(int), 1, f);
        fwrite(&joueurs[i].nb_defausse, sizeof(int), 1, f);
        fwrite(joueurs[i].cartes, sizeof(Carte), joueurs[i].nb_cartes, f);
        fwrite(joueurs[i].defausse, sizeof(Carte), joueurs[i].nb_defausse, f);
    }

    fclose(f);
    return 1;
}

int charger_partie(const char *fichier, Joueur *joueurs, int *nb_joueurs, int *nb_cartes, Carte *pioche, int *nb_pioche) {
    FILE *f = fopen(fichier, "rb");
    if (!f) return 0;

    fread(nb_joueurs, sizeof(int), 1, f);
    fread(nb_cartes, sizeof(int), 1, f);
    fread(nb_pioche, sizeof(int), 1, f);
    fread(pioche, sizeof(Carte), *nb_pioche, f);

    for (int i = 0; i < *nb_joueurs; i++) {
        fread(joueurs[i].nom, sizeof(char), MAX_NOM, f);
        fread(&joueurs[i].nb_cartes, sizeof(int), 1, f);
        fread(&joueurs[i].nb_defausse, sizeof(int), 1, f);
        fread(joueurs[i].cartes, sizeof(Carte), joueurs[i].nb_cartes, f);
        fread(joueurs[i].defausse, sizeof(Carte), joueurs[i].nb_defausse, f);
    }

    fclose(f);
    return 1;
}
