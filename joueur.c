// joueurs.c
#include <stdio.h>
#include <string.h>
#include "joueur.h"
#include "cartes.h"

void initialiser_joueurs(Joueur *joueurs, int nb_joueurs, int nb_cartes_par_joueur) {
    for (int i = 0; i < nb_joueurs; i++) {
        printf("Nom du joueur %d : ", i + 1);
        scanf("%s", joueurs[i].nom);
        joueurs[i].nb_cartes = nb_cartes_par_joueur;
        joueurs[i].nb_defausse = 0;
        // Les cartes seront affectées plus tard lors de la distribution.
    }
}

void afficher_joueur(const Joueur *joueur) {
    printf("Joueur : %s\n", joueur->nom);
    printf("Main : ");
    for (int i = 0; i < joueur->nb_cartes; i++) {
        // Affiche chaque carte en appelant la fonction d'affichage (définie dans display.c ou ici directement)
        if (joueur->cartes[i].visible)
            printf("[%d] ", joueur->cartes[i].valeur);
        else
            printf("[?] ");
    }
    printf("\nDéfausse : ");
    for (int i = 0; i < joueur->nb_defausse; i++) {
        printf("[%d] ", joueur->defausse[i].valeur);
    }
    printf("\n");
}

void reveler_joueur(Joueur *joueur) {
    for (int i = 0; i < joueur->nb_cartes; i++) {
        joueur->cartes[i].visible = 1;
    }
}

int calculer_score(const Joueur *joueur) {
    int score = 0;
    for (int i = 0; i < joueur->nb_cartes; i++) {
        score += joueur->cartes[i].valeur;
    }
    return score;
}
