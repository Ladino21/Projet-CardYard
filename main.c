#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include "display.h"
#include "save.h"

int main() {
    Jeu jeu;
    int choix;
    srand(time(NULL));
    do {
        choix = afficherMenu();
        switch (choix) {
            case 1: {
                int nb;
                printf("Entrez le nombre de joueurs (2-8) : ");
                if (scanf("%d", &nb) != 1 || nb < 2) {
                    printf("Nombre de joueurs invalide.\n");
                    // Vider le tampon d'entrée
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF) {}
                    break;
                }
                if (nb > 8) {
                    nb = 8;
                }
                // Vider le reste de la ligne d'entrée
                int c;
                while ((c = getchar()) != '\n' && c != EOF) {}
                initialiserJeu(&jeu, nb);
                if (jouerPartie(&jeu) == 1) {
                    // Partie sauvegardée et interrompue
                    libererJoueurs(jeu.joueurs, jeu.nb_joueurs);
                    free(jeu.pioche);
                    return 0;
                }
                // Partie terminée normalement
                libererJoueurs(jeu.joueurs, jeu.nb_joueurs);
                free(jeu.pioche);
                break;
            }
            case 2:
                if (chargerJeu("sauvegarde.txt", &jeu) == 0) {
                    printf("Partie chargée avec succès.\n");
                    if (jouerPartie(&jeu) == 1) {
                        libererJoueurs(jeu.joueurs, jeu.nb_joueurs);
                        free(jeu.pioche);
                        return 0;
                    }
                    libererJoueurs(jeu.joueurs, jeu.nb_joueurs);
                    free(jeu.pioche);
                } else {
                    printf("Échec du chargement de la partie.\n");
                }
                break;
            case 3:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide, veuillez réessayer.\n");
                break;
        }
    } while (choix != 3);
    return 0;
}
