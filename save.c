//adam

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "cartes.h"
#include "joueurs.h"

int sauvegarderPartie(const char *nom_fichier, const Partie *partie) {
    //verif
    if (partie == NULL || nom_fichier == NULL) return -1;

    FILE *f = fopen(nom_fichier, "wb");
    if (f==NULL) return -1;

    // Écrire les informations principales de la partie
    fwrite(&partie->nb_joueurs, sizeof(int), 1, f);
    fwrite(&partie->nb_cartes_personnelles, sizeof(int), 1, f);
    fwrite(&partie->joueur_courant, sizeof(int), 1, f);

    // Sauvegarder la pioche
    fwrite(&partie->pioche.taille, sizeof(int), 1, f);
    if (partie->pioche.taille > 0) {
        fwrite(partie->pioche.cartes, sizeof(Carte), partie->pioche.taille, f);
    }

    // Sauvegarder les données de chaque joueur
    for (int i = 0; i < partie->nb_joueurs; ++i) {
        fwrite(&partie->joueurs[i].nb_cartes, sizeof(int), 1, f);
        if (partie->joueurs[i].nb_cartes > 0) {
            fwrite(partie->joueurs[i].personnelles, sizeof(Carte), partie->joueurs[i].nb_cartes, f);
        }

        fwrite(&partie->joueurs[i].nb_defausse, sizeof(int), 1, f);
        if (partie->joueurs[i].nb_defausse > 0) {
            fwrite(partie->joueurs[i].defausse, sizeof(Carte), partie->joueurs[i].nb_defausse, f);
        }
    }

    fclose(f);
    return 0;
}

Partie* chargerPartie(const char *nom_fichier) {
    if (nom_fichier==NULL) return NULL;

    FILE *f = fopen(nom_fichier, "rb");
    if (f==NULL) return NULL;

    Partie *partie = malloc(sizeof(Partie));
    if (partie==NULL) {
        fclose(f);
        return NULL;
    }
//verification pour ne pas rentrer n'importe quoi dans fichier 
    if (fread(&partie->nb_joueurs, sizeof(int), 1, f) != 1) {
        fclose(f);
        free(partie);
        return NULL;
    }
    //la meme chose qu'avant
    if (fread(&partie->nb_cartes_personnelles, sizeof(int), 1, f) != 1) {
        fclose(f);
        free(partie);
        return NULL;
    }

    
    if (fread(&partie->joueur_courant, sizeof(int), 1, f) != 1) {
        fclose(f);
        free(partie);
        return NULL;
    }


    // Lire la pioche
    int taille_pioche = 0;
    fread(&taille_pioche, sizeof(int), 1, f);
    partie->pioche.taille = taille_pioche;

    if (taille_pioche > 0) {
        partie->pioche.cartes = malloc(taille_pioche * sizeof(Carte));
        if (partie->pioche.cartes==NULL) {
            fclose(f);
            free(partie);
            return NULL;
        }
        //verif encore du fichier
        if (fread(partie->pioche.cartes, sizeof(Carte), taille_pioche, f) != taille_pioche) {
            fclose(f);
            free(partie->pioche.cartes);
            free(partie);
            return NULL;
        }

    } else {
        partie->pioche.cartes = NULL;
    }

    // Allouer les joueurs
    partie->joueurs = malloc(partie->nb_joueurs * sizeof(Joueur));
    if (partie->joueurs==NULL) {
        if (partie->pioche.cartes) free(partie->pioche.cartes);
        free(partie);
        fclose(f);
        return NULL;
    }

    // Calculer les tailles initiales pour les piles de défausse
    int total_personnelles = partie->nb_joueurs * partie->nb_cartes_personnelles;
    int total_defausse = 0;

    fpos_t pos;
    fgetpos(f, &pos);
    for (int i = 0; i < partie->nb_joueurs; ++i) {
        int nb_personnelles, nb_defausse;
        fread(&nb_personnelles, sizeof(int), 1, f);
        fseek(f, nb_personnelles * sizeof(Carte), SEEK_CUR);
        fread(&nb_defausse, sizeof(int), 1, f);
        total_defausse += nb_defausse;
        fseek(f, nb_defausse * sizeof(Carte), SEEK_CUR);
    }
    fsetpos(f, &pos);

    int capacite_totale = taille_pioche + total_personnelles + total_defausse;
    if (capacite_totale < 1) capacite_totale = 1;

    // Lire les données des joueurs
    for (int i = 0; i < partie->nb_joueurs; ++i) {
        int nb_cartes;
        //verif encore
        if (fread(&nb_cartes, sizeof(int), 1, f) != 1) {
            fclose(f);
            free(partie);
            return NULL;
        }
        
        partie->joueurs[i].nb_cartes = nb_cartes;

        partie->joueurs[i].personnelles = malloc(nb_cartes * sizeof(Carte));
        if (partie->joueurs[i].personnelles==NULL) {
            for (int k = 0; k < i; ++k) {
                free(partie->joueurs[k].personnelles);
                free(partie->joueurs[k].defausse);
            }
            free(partie->joueurs);
            if (partie->pioche.cartes) free(partie->pioche.cartes);
            free(partie);
            fclose(f);
            return NULL;
        }
        if (nb_cartes > 0) {
            fread(partie->joueurs[i].personnelles, sizeof(Carte), nb_cartes, f);
        }

        fread(&partie->joueurs[i].nb_defausse, sizeof(int), 1, f);
        int nb_defausse = partie->joueurs[i].nb_defausse;

        partie->joueurs[i].defausse = malloc(capacite_totale * sizeof(Carte));
        if (partie->joueurs[i].defausse==NULL) {
            free(partie->joueurs[i].personnelles);
            for (int k = 0; k < i; ++k) {
                free(partie->joueurs[k].personnelles);
                free(partie->joueurs[k].defausse);
            }
            free(partie->joueurs);
            if (partie->pioche.cartes) free(partie->pioche.cartes);
            free(partie);
            fclose(f);
            return NULL;
        }
        if (nb_defausse > 0) {
            fread(partie->joueurs[i].defausse, sizeof(Carte), nb_defausse, f);
        }
    }

    fclose(f);
    return partie;
}
