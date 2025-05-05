// === joueurs.c ===
#include <stdio.h>
#include <string.h>
#include "joueurs.h"

void initialiser_joueurs(Joueur *joueurs, int nb_joueurs, int nb_cartes, Carte *pioche, int *nb_pioche) {
    for (int i = 0; i < nb_joueurs; i++) {
        printf("Nom du joueur %d : ", i + 1);
        scanf("%s", joueurs[i].nom);
        joueurs[i].nb_cartes = nb_cartes;
        joueurs[i].nb_defausse = 0;
        for (int j = 0; j < nb_cartes; j++) {
            joueurs[i].cartes[j] = pioche[--(*nb_pioche)];
            joueurs[i].cartes[j].visible = 0;
        }
    }
}

void afficher_joueur(const Joueur *j) {
    printf("%s\nCartes : ", j->nom);
    for (int i = 0; i < j->nb_cartes; i++) {
        afficher_carte(j->cartes[i]);
        printf(" ");
    }
    printf("\nDéfausse : ");
    for (int i = 0; i < j->nb_defausse; i++) {
        printf("[%2d] ", j->defausse[i].valeur);
    }
    printf("\n");
}

void ajouter_defausse(Joueur *j, Carte c) {
    if (j->nb_defausse < MAX_DEFAUSSE)
        j->defausse[j->nb_defausse++] = c;
}

int calculer_score(Joueur *j) {
    int total = 0;
    for (int i = 0; i < j->nb_cartes; i++)
        total += j->cartes[i].valeur;
    return total;
}

int toutes_visibles(Joueur *j) {
    for (int i = 0; i < j->nb_cartes; i++)
        if (!j->cartes[i].visible)
            return 0;
    return 1;
}
