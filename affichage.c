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
        case -1: return "\033[105m"; // magenta clair fond plein
        case  0: return "\033[44m";
        case  1: return "\033[46m";
        case  2: return "\033[106m";
        case  3: return "\033[102m";
        case  4: return "\033[42m";
        case  5: return "\033[42m";  // vert fond plein
        case  6: return "\033[103m";
        case  7: return "\033[43m";
        case  8: return "\033[101m";
        case  9: return "\033[101m"; // rouge clair fond plein
        case 10: return "\033[41m";
        case 11: return "\033[41m";  // rouge fond plein
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

    for (int i = 0; i < partie->nb_joueurs - 1; ++i) {
        for (int j = i + 1; j < partie->nb_joueurs; ++j) {
            if (scores[i].score > scores[j].score) {
                Classement tmp = scores[i];
                scores[i] = scores[j];
                scores[j] = tmp;
            }
        }
    }

    printf("\n !! Classement final !! :\n");
    for (int i = 0; i < partie->nb_joueurs; ++i) {
        printf("  %d* Joueur %d - Score : %d\n", i + 1, scores[i].joueur_id, scores[i].score);
    }
}

void afficherLigneCartesAvecIndice(const Carte *cartes, int nbCartes, int indice_debut) {
    //verif pointeur
    if (cartes == NULL) {
        printf("Carte invalide (NULL)\n");
        return;
    }
    
    if (nbCartes <= 0) return;

    const int max_par_ligne = 10;

    for (int ligne = 0; ligne * max_par_ligne < nbCartes; ++ligne) {
        int debut = ligne * max_par_ligne;
        int fin = debut + max_par_ligne;
        if (fin > nbCartes) fin = nbCartes;

        // Ligne haut
        for (int i = debut; i < fin; ++i) {
            const char *col = cartes[i].visible ? couleurCarte(cartes[i].valeur) : "\033[100m";
            printf("%s+-------+\033[0m ", col);
        }
        printf("\n");

        // Ligne texte haut
        for (int i = debut; i < fin; ++i) {
            const Carte *c = &cartes[i];
            const char *col = c->visible ? couleurCarte(c->valeur) : "\033[100m";
            printf("%s|%s", col, col);
            if (!c->visible) printf(" CARD  ");
            else printf("       ");
            printf("%s|\033[0m ", col);
        }
        printf("\n");

        // Ligne texte bas (valeurs)
        for (int i = debut; i < fin; ++i) {
            const Carte *c = &cartes[i];
            const char *col = c->visible ? couleurCarte(c->valeur) : "\033[100m";
            printf("%s|%s", col, col);
            if (!c->visible) printf(" YARD  ");
            else printf("  %2d   ", c->valeur);
            printf("%s|\033[0m ", col);
        }
        printf("\n");

        // Ligne bas
        for (int i = debut; i < fin; ++i) {
            const char *col = cartes[i].visible ? couleurCarte(cartes[i].valeur) : "\033[100m";
            printf("%s+-------+\033[0m ", col);
        }
        printf("\n");

        // Ligne des indices
        for (int i = debut; i < fin; ++i) {
            int index = i + indice_debut;
            if (index < 10){
                printf("   [%d]    ", index);
            } else {
                printf("  [%d]    ", index);
            }
        }
        printf("\n\n");
    }
}

void afficherCarteStylisee(const Carte *carte) {
    //verif
    if (carte == NULL) {
        printf("Carte invalide (NULL)\n");
        return;
    }
    const char *col = carte->visible ? couleurCarte(carte->valeur) : "\033[100m";

    printf("%s+-------+\033[0m\n", col);
    printf("%s|%s", col, col);
    if (!carte->visible) {
        printf("  ??   ");
    } else {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", carte->valeur);
        int len = strlen(buf);
        int pad = (7 - len) / 2;
        printf("%*s%s%*s", pad, "", buf, 7 - len - pad, "");
    }
    printf("%s|\033[0m\n", col);
    printf("%s+-------+\033[0m\n", col);
}

void afficherPiocheCentrale(const Pioche *pioche) {
    //verif
    if (pioche == NULL) {
        printf("pioche invalide (NULL)\n");
        return;
    }
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
    //verif sur les pointeurs
    if (joueur == NULL) {
        printf("joueur invalide (NULL)\n");
        return;
    }
    
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
            const char *col = joueur->defausse[i].visible ? couleurCarte(joueur->defausse[i].valeur) : "\033[100m";
            printf("%s+-------+\033[0m ", col);
        }
        printf("\n");

        // Ligne milieu
        for (int i = debut; i < fin; ++i) {
            const Carte *c = &joueur->defausse[i];
            const char *col = c->visible ? couleurCarte(c->valeur) : "\033[100m";
            printf("%s|%s", col, col);
            if (!c->visible)
                printf("  ??   ");
            else
                printf("  %2d   ", c->valeur);
            printf("%s|\033[0m ", col);
        }
        printf("\n");

        // Ligne bas
        for (int i = debut; i < fin; ++i) {
            const char *col = joueur->defausse[i].visible ? couleurCarte(joueur->defausse[i].valeur) : "\033[100m";
            printf("%s+-------+\033[0m ", col);
        }
        printf("\n");

        // Indices centrés
        for (int i = debut; i < fin; ++i) {
            if (i < 10){
                printf("   [%d]    ", i); // 9 caractères
            }
            else{
                printf("  [%d]    ", i);  // aussi 9 caractère
            }
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
        afficherLigneCartesAvecIndice(partie->joueurs[i].personnelles, partie->joueurs[i].nb_cartes,0);
        afficherDefausse(&partie->joueurs[i]);
        printf("\n");
    }
}
