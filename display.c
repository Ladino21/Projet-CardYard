#include <stdio.h>
#include "display.h"

// Affiche le menu principal et lit le choix de l'utilisateur
int afficherMenu() {
    int choix;
    printf("=== Menu Principal ===\n");
    printf("1. Nouvelle partie\n");
    printf("2. Charger une partie\n");
    printf("3. Quitter\n");
    printf("Votre choix : ");
    if (scanf("%d", &choix) != 1) {
        choix = -1;
    }
    // Vider le tampon d'entrée
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    return choix;
}

// Affiche l'état de jeu courant
void afficherJeu(const Jeu *jeu, int joueurActuel) {
    // Pioche centrale
    printf("Pioche centrale : %d carte(s) restante(s)\n", jeu->nb_cartes_pioche);
    // Pour chaque joueur, afficher la défausse et les cartes personnelles
    for (int j = 0; j < jeu->nb_joueurs; ++j) {
        if (jeu->joueurs[j].nb_defausse > 0) {
            Carte top = jeu->joueurs[j].defausse[jeu->joueurs[j].nb_defausse - 1];
            printf("Défausse Joueur %d : [%d]\n", j + 1, top.valeur);
        } else {
            printf("Défausse Joueur %d : [vide]\n", j + 1);
        }
        printf("Joueur %d - Cartes : ", j + 1);
        for (int i = 0; i < NB_CARTES_PERSONNELLES; ++i) {
            afficherCarte(&jeu->joueurs[j].cartes_personnelles[i], jeu->joueurs[j].cartes_visibles[i]);
            printf(" ");
        }
        printf("\n");
    }
}

// Affiche une carte individuelle
void afficherCarte(const Carte *carte, bool visible) {
    if (visible) {
        // Afficher la valeur (avec un espace si nombre positif pour aligner)
        if (carte->valeur >= 0) {
            printf("%2d", carte->valeur);
        } else {
            printf("%d", carte->valeur);
        }
    } else {
        printf("XX");
    }
}
