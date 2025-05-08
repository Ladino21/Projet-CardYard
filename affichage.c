#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"
#include "cartes.h"
#include "joueurs.h"

// Affiche une ligne de cartes personnelles
void afficherLigneCartes(const Carte *cartes, int nbCartes) {
    if (nbCartes <= 0) return;
    const int largeur = 4;

    // Bord supérieur
    for (int j = 0; j < nbCartes; j++) {
        printf("+------+" );
        if (j < nbCartes - 1) printf(" ");
    }
    printf("\n");

    // Ligne avec les valeurs
    for (int j = 0; j < nbCartes; j++) {
        const Carte *carte = &cartes[j];
        printf("| ");
        if (!carte->visible) {
            printf(" ?? ");
        } else {
            if (carte->valeur < 0 || carte->valeur > 99) {
                printf(" ?? ");
            } else {
                printf("%2d ", carte->valeur);
            }
        }
        printf("|");
        if (j < nbCartes - 1) printf(" ");
    }
    printf("\n");

    // Bord inférieur
    for (int j = 0; j < nbCartes; j++) {
        printf("+------+" );
        if (j < nbCartes - 1) printf(" ");
    }
    printf("\n");
}

// Affiche l'état de la partie
void afficherPartie(const Partie *partie) {
    if (!partie) return;
    for (int p = 0; p < partie->nb_joueurs; ++p) {
        printf("Joueur %d :\n", p + 1);
        afficherLigneCartes(partie->joueurs[p].personnelles, partie->joueurs[p].nb_cartes);

        // Indices en dessous des cartes
        if (partie->joueurs[p].nb_cartes > 0) {
            for (int j = 0; j < partie->joueurs[p].nb_cartes; ++j) {
                printf("   %d   ", j);
                if (j < partie->joueurs[p].nb_cartes - 1) printf(" ");
            }
            printf("\n");
        }

        // Défausse
        printf("Défausse : ");
        if (partie->joueurs[p].nb_defausse > 0) {
            Carte sommet = partie->joueurs[p].defausse[partie->joueurs[p].nb_defausse - 1];
            afficherLigneCartes(&sommet, 1);
        } else {
            printf("(vide)\n");
        }

        printf("\n");
    }
}
