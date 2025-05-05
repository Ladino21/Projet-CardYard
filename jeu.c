// === jeu.c ===
#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"
#include "cartes.h"
#include "joueurs.h"

void menu_principal() {
    int choix;
    do {
        printf("\n===== CARD YARD =====\n");
        printf("1. Nouvelle partie\n");
        printf("2. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                lancer_partie();
                break;
            case 2:
                printf("\nÀ bientôt !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 2);
}

void lancer_partie() {
    int nb_joueurs, nb_cartes;
    Joueur joueurs[MAX_JOUEURS];
    Carte pioche[MAX_CARTES];
    int nb_pioche;

    printf("\nNombre de joueurs (2-8) : ");
    scanf("%d", &nb_joueurs);
    printf("Nombre de cartes par joueur : ");
    scanf("%d", &nb_cartes);

    if (nb_joueurs * nb_cartes > MAX_CARTES - 75) {
        printf("Trop de cartes distribuées.\n");
        return;
    }

    initialiser_pioche(pioche, &nb_pioche);
    melanger_pioche(pioche, nb_pioche);
    initialiser_joueurs(joueurs, nb_joueurs, nb_cartes, pioche, &nb_pioche);

    int fin = 0;
    while (!fin) {
        for (int i = 0; i < nb_joueurs; i++) {
            printf("\n--- Tour de %s ---\n", joueurs[i].nom);
            afficher_joueur(&joueurs[i]);

            if (nb_pioche <= 0) {
                printf("La pioche est vide.\n");
                continue;
            }

            Carte tirage = piocher_carte(pioche, &nb_pioche);
            printf("Carte piochée : ");
            afficher_carte(tirage);
            printf("\nIndex de la carte à échanger (0-%d, -1 pour défausser) : ", nb_cartes - 1);
            int idx;
            scanf("%d", &idx);

            if (idx == -1) {
                ajouter_defausse(&joueurs[i], tirage);
            } else if (idx >= 0 && idx < nb_cartes && !joueurs[i].cartes[idx].visible) {
                Carte remplacee = joueurs[i].cartes[idx];
                remplacee.visible = 1;
                joueurs[i].cartes[idx] = tirage;
                joueurs[i].cartes[idx].visible = 1;
                ajouter_defausse(&joueurs[i], remplacee);
            } else {
                printf("Échange invalide.\n");
            }

            if (toutes_visibles(&joueurs[i])) {
                fin = 1;
                break;
            }
        }
    }

    printf("\n--- Fin de partie ---\n");
    for (int i = 0; i < nb_joueurs; i++) {
        for (int j = 0; j < joueurs[i].nb_cartes; j++) {
            joueurs[i].cartes[j].visible = 1;
        }
        afficher_joueur(&joueurs[i]);
        printf("Score de %s : %d\n", joueurs[i].nom, calculer_score(&joueurs[i]));
    }
}
