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
void afficherClassement(const Partie *partie) {
    typedef struct {
        int joueur_id;
        int score;
    } Classement;

    Classement scores[NB_JOUEURS_MAX];

    for (int i = 0; i < partie->nb_joueurs; ++i) {
        int total = 0;
        for (int j = 0; j < partie->joueurs[i].nb_cartes; ++j) {
            total += partie->joueurs[i].personnelles[j].valeur;
        }
        scores[i].joueur_id = i + 1;
        scores[i].score = total;
    }

    // Tri des scores (ordre croissant)
    for (int i = 0; i < partie->nb_joueurs - 1; ++i) {
        for (int j = i + 1; j < partie->nb_joueurs; ++j) {
            if (scores[i].score > scores[j].score) {
                Classement tmp = scores[i];
                scores[i] = scores[j];
                scores[j] = tmp;
            }
        }
    }

    printf("\n🏆 Classement final :\n");
    for (int i = 0; i < partie->nb_joueurs; ++i) {
        printf("  %d. Joueur %d - Score : %d\n", i + 1, scores[i].joueur_id, scores[i].score);
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
    const char *couleur = (carte->visible && carte->valeur >= -2 && carte->valeur <= 99)
                          ? couleurCarte(carte->valeur)
                          : "\033[100m";

    // Bord supérieur
    printf("%s+-------+\033[0m\n", couleur);

    // Ligne centrale avec valeur centrée ou "??"
    printf("%s|", couleur);
    if (!carte->visible) {
        printf("  ??   ");
    } else {
        char buffer[8];
        snprintf(buffer, sizeof(buffer), "%d", carte->valeur);
        int len = strlen(buffer);
        int esp_gauche = (7 - len) / 2;
        int esp_droite = 7 - len - esp_gauche;
        printf("%*s%s%*s", esp_gauche, "", buffer, esp_droite, "");
    }
    printf("|\033[0m\n");

    // Bord inférieur
    printf("%s+-------+\033[0m\n", couleur);
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




void afficherDefausse(const Joueur *joueur) {
    printf("Défausse :\n");

    if (joueur->nb_defausse == 0) {
        printf("(vide)\n");
        return;
    }

    const int max_par_ligne = 5;
    int total = joueur->nb_defausse;

    for (int ligne = 0; ligne * max_par_ligne < total; ++ligne) {
        int debut = ligne * max_par_ligne;
        int fin = debut + max_par_ligne;
        if (fin > total) fin = total;

        // Ligne haut
        for (int i = debut; i < fin; ++i) {
            const char *couleur = joueur->defausse[i].visible ? couleurCarte(joueur->defausse[i].valeur) : "\033[100m";
            printf("%s+-------+\033[0m ", couleur);
        }
        printf("\n");

        // Ligne milieu
        for (int i = debut; i < fin; ++i) {
            const Carte *carte = &joueur->defausse[i];
            const char *couleur = carte->visible ? couleurCarte(carte->valeur) : "\033[100m";
            printf("%s|", couleur);
            if (!carte->visible) {
                printf("  ??   ");
            } else {
                printf("  %2d   ", carte->valeur);
            }
            printf("|\033[0m ");
        }
        printf("\n");

        // Ligne bas
        for (int i = debut; i < fin; ++i) {
            const char *couleur = joueur->defausse[i].visible ? couleurCarte(joueur->defausse[i].valeur) : "\033[100m";
            printf("%s+-------+\033[0m ", couleur);
        }
        printf("\n");

        // Indices
        for (int i = debut; i < fin; ++i) {
            printf("   %2d   ", i);
        }
        printf("\n\n");
    }
}




void afficherPartie(const Partie *partie) {
    if (!partie) return;

    printf("\n==== État du jeu ====\n\n");
    afficherPiocheCentrale(&partie->pioche);

    for (int i = 0; i < partie->nb_joueurs; ++i) {
        printf("Joueur %d :\n", i + 1);
        afficherLigneCartes(partie->joueurs[i].personnelles, partie->joueurs[i].nb_cartes);
        afficherDefausse(&partie->joueurs[i]);

        printf("\n");
    }
}
