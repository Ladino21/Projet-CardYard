#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "cartes.h"
#include "joueurs.h"

#define LARGEUR_CARTE 9
#define TAILLE_TERMINAL 100

const char* couleurCarte(int valeur) {
    switch(valeur) {
        case -2: return "\033[45m"; // Magenta
        case -1: return "\033[35m"; // Violet
        case  0: return "\033[44m";
        case  1: return "\033[46m";
        case  2: return "\033[106m";
        case  3: return "\033[102m";
        case  4: return "\033[42m";
        case  5: return "\033[32m";
        case  6: return "\033[103m";
        case  7: return "\033[43m";
        case  8: return "\033[101m";
        case  9: return "\033[91m";
        case 10: return "\033[41m";
        case 11: return "\033[31m";
        case 12: return "\033[41m";
        default: return "\033[47m";
    }
}

void afficherEspacesCentrage(int nbCartes) {
    int totalLargeur = nbCartes * (LARGEUR_CARTE + 1);
    int marge = (TAILLE_TERMINAL - totalLargeur) / 2;
    for (int i = 0; i < marge; i++) {
        printf(" ");
    }
}

void afficherLigneCartes(const Carte *cartes, int nbCartes) {
    if (nbCartes <= 0) return;

    // Bord supérieur
    for (int j = 0; j < nbCartes; j++) {
        const Carte *c = &cartes[j];
        printf("%s+-------+\033[0m", c->visible ? couleurCarte(c->valeur) : "\033[100m");
    }
    printf("\n");

    // Ligne centrale
    for (int j = 0; j < nbCartes; j++) {
        const Carte *c = &cartes[j];
        const char *couleur = c->visible ? couleurCarte(c->valeur) : "\033[100m";
        printf("%s|", couleur);
        if (!c->visible) {
            printf("  ??   ");
        } else {
            printf("  %2d   ", c->valeur);
        }
        printf("|\033[0m");
    }
    printf("\n");

    // Bord inférieur
    for (int j = 0; j < nbCartes; j++) {
        const Carte *c = &cartes[j];
        printf("%s+-------+\033[0m", c->visible ? couleurCarte(c->valeur) : "\033[100m");
    }
    printf("\n");

    // Indices centrés sous chaque carte
    for (int j = 0; j < nbCartes; j++) {
        printf("   %2d   ", j);
    }
    printf("\n");
}

void afficherCarteStylisee(const Carte *carte) {
    const char *couleur = (carte->visible && carte->valeur >= -2 && carte->valeur <= 99) ? couleurCarte(carte->valeur) : "\033[100m";
    printf("%s+-------+\033[0m\n", couleur);
    printf("%s| ", couleur);
    if (!carte->visible) {
        printf(" ?? ");
    } else {
        printf(" %2d ", carte->valeur);
    }
    printf(" |\033[0m\n");
    printf("%s+-------+\033[0m\n", couleur);
}

void afficherPiocheCentrale(const Pioche *pioche) {
    printf("Pioche centrale :\n");
    if (pioche->taille == 0) {
        printf("(vide)\n");
    } else {
        Carte dos = { .valeur = -1, .visible = false };
        afficherCarteStylisee(&dos);
    }
    printf("\n");
}

void afficherDefausse(const Joueur *joueur) {
    printf("Défausse : ");
    if (joueur->nb_defausse == 0) {
        printf("(vide)\n");
    } else {
        Carte top = joueur->defausse[joueur->nb_defausse - 1];
        printf("\n");
        afficherCarteStylisee(&top);
    }
}

void afficherPartie(const Partie *partie) {
    if (!partie) return;

    printf("\n==== État du jeu ====\n\n");
    afficherPiocheCentrale(&partie->pioche);
    printf("\n");

    for (int i = 0; i < partie->nb_joueurs; ++i) {
        printf("Joueur %d :\n", i + 1);
        afficherLigneCartes(partie->joueurs[i].personnelles, partie->joueurs[i].nb_cartes);
        afficherDefausse(&partie->joueurs[i]);
        printf("\n");
    }
}
