#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "cartes.h"
#include "joueurs.h"



#define TAILLE_TERMINAL 100
#define LARGEUR_CARTE 9

const char* couleurCarte(int valeur) {
    switch (valeur) {
        case -2: return "\033[45m";
        case -1: return "\033[35m";
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

void afficherLigneCartes(const Carte *cartes, int nbCartes) {
    if (nbCartes <= 0) return;

    int largeur_totale = nbCartes * LARGEUR_CARTE;
    int offset = (TAILLE_TERMINAL - largeur_totale) / 2;
    if (offset < 0) offset = 0;

    // Ligne du haut
    for (int i = 0; i < offset; i++) printf(" ");
    for (int j = 0; j < nbCartes; j++) {
        const Carte *c = &cartes[j];
        const char *col = c->visible ? couleurCarte(c->valeur) : "\033[100m";
        printf("%s+-------+\033[0m", col);
    }
    printf("\n");

    // Ligne 1 du contenu
    for (int i = 0; i < offset; i++) printf(" ");
    for (int j = 0; j < nbCartes; j++) {
        const Carte *c = &cartes[j];
        const char *col = c->visible ? couleurCarte(c->valeur) : "\033[100m";
        printf("%s|", col);
        if (!c->visible)
            printf(" CARD  ");
        else
            printf("       ");
        printf("|\033[0m");
    }
    printf("\n");

    // Ligne 2 du contenu
    for (int i = 0; i < offset; i++) printf(" ");
    for (int j = 0; j < nbCartes; j++) {
        const Carte *c = &cartes[j];
        const char *col = c->visible ? couleurCarte(c->valeur) : "\033[100m";
        printf("%s|", col);
        if (!c->visible)
            printf(" YARD  ");
        else
            printf("  %2d   ", c->valeur);
        printf("|\033[0m");
    }
    printf("\n");

    // Ligne du bas
    for (int i = 0; i < offset; i++) printf(" ");
    for (int j = 0; j < nbCartes; j++) {
        const Carte *c = &cartes[j];
        const char *col = c->visible ? couleurCarte(c->valeur) : "\033[100m";
        printf("%s+-------+\033[0m", col);
    }
    printf("\n");

    // Indices
    for (int i = 0; i < offset; i++) printf(" ");
    for (int j = 0; j < nbCartes; j++) {
        printf("   [%d]   ", j);
    }
    printf("\n");
}

void afficherCarteStylisee(const Carte *carte) {
    const char *col = carte->visible ? couleurCarte(carte->valeur) : "\033[100m";

    printf("%s+-------+\033[0m\n", col);
    printf("%s|", col);
    if (!carte->visible) printf(" CARD  "); else printf("       ");
    printf("|\033[0m\n");

    printf("%s|", col);
    if (!carte->visible) printf(" YARD  "); else printf("  %2d   ", carte->valeur);
    printf("|\033[0m\n");

    printf("%s+-------+\033[0m\n", col);
}

void afficherPiocheCentrale(const Pioche *pioche) {
    printf("\nPioche centrale :\n");
    if (pioche->taille == 0) {
        printf("        (vide)\n");
    } else {
        Carte dos = { .valeur = -1, .visible = false };
        afficherCarteStylisee(&dos);
    }
    printf("\n");
}

void afficherDefausse(const Joueur *joueur, int nbCartes) {
    int largeur_totale = LARGEUR_CARTE;
    int offset = (TAILLE_TERMINAL - largeur_totale) / 2;
    if (offset < 0) offset = 0;

    // Ligne centrée "Défausse :"
    for (int i = 0; i < offset; i++) printf(" ");
    printf("Défausse :\n");

    if (joueur->nb_defausse == 0) {
        for (int i = 0; i < offset; i++) printf(" ");
        printf("(vide)\n");
    } else {
        Carte *carte = &joueur->defausse[joueur->nb_defausse - 1];
        const char *col = carte->visible ? couleurCarte(carte->valeur) : "\033[100m";

        for (int i = 0; i < offset; i++) printf(" ");
        printf("%s+-------+\033[0m\n", col);

        for (int i = 0; i < offset; i++) printf(" ");
        printf("%s|", col);
        if (!carte->visible) printf(" CARD  ");
        else printf("       ");
        printf("|\033[0m\n");

        for (int i = 0; i < offset; i++) printf(" ");
        printf("%s|", col);
        if (!carte->visible) printf(" YARD  ");
        else printf("  %2d   ", carte->valeur);
        printf("|\033[0m\n");

        for (int i = 0; i < offset; i++) printf(" ");
        printf("%s+-------+\033[0m\n", col);
    }
}
void afficherPartie(const Partie *partie) {
    if (!partie) return;

    printf("\n==== État du jeu ====\n\n");
    afficherPiocheCentrale(&partie->pioche);

    for (int i = 0; i < partie->nb_joueurs; ++i) {
        printf("Joueur %d :\n", i + 1);
        afficherLigneCartes(partie->joueurs[i].personnelles, partie->joueurs[i].nb_cartes);
        afficherDefausse(&partie->joueurs[i], partie->joueurs[i].nb_cartes);
        printf("\n");
    }
}
