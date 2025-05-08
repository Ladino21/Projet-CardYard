#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "save.h"

int main() {
    printf(" Bienvenue dans CARD YARD !!\n");
    while (1) {
        printf("\nMenu principal:\n");
        printf("1. Nouvelle partie\n");
        printf("2. Charger partie\n");
        printf("3. Quitter\n");
        printf("Votre choix: ");
        int choix = 0;
        if (scanf("%d", &choix) != 1) {
            // Si l'entrée n'est pas un entier (EOF par ex.), on quitte
            break;
        }
        // Vider le tampon jusqu'au prochain saut de ligne
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        if (choix == 1) {
            // Nouvelle partie
            int numPlayers = 0;
            int cartesParJoueur = 0;
            char fileChoice;
            char filename[256] = "";
            // Saisie du nombre de joueurs
            while (numPlayers < 2 || numPlayers > 8) {
                printf("Entrez le nombre de joueurs (2-8): ");
                if (scanf("%d", &numPlayers) != 1) {
                    numPlayers = 0;
                    // Consommer la fin de ligne incorrecte
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    continue;
                }
                while ((ch = getchar()) != '\n' && ch != EOF) {}
                if (numPlayers < 2 || numPlayers > 8) {
                    printf("Nombre de joueurs invalide. Veuillez recommencer.\n");
                }
            }
            // Saisie du nombre de cartes personnelles par joueur
            while (cartesParJoueur < 1) {
                printf("Entrez le nombre de cartes personnelles par joueur: ");
                if (scanf("%d", &cartesParJoueur) != 1) {
                    cartesParJoueur = 0;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    continue;
                }
                while ((ch = getchar()) != '\n' && ch != EOF) {}
                if (cartesParJoueur < 1) {
                    printf("Nombre de cartes invalide.\n");
                }
            }
            // Choix de chargement de la pioche depuis un fichier
            printf("Voulez-vous charger la pioche depuis un fichier? (o/n): ");
            if (scanf(" %c", &fileChoice) != 1) {
                fileChoice = 'n';
            }
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            if (fileChoice == 'o' || fileChoice == 'O') {
                printf("Entrez le nom du fichier de pioche: ");
                if (fgets(filename, sizeof(filename), stdin)) {
                    // Retirer le saut de ligne final
                    filename[strcspn(filename, "\n")] = '\0';
                }
            }
            // Création du jeu
            Game *game = createGame(numPlayers, cartesParJoueur, 
                                    (fileChoice=='o'||fileChoice=='O') ? filename : NULL);
            if (!game) {
                printf("Impossible de créer la partie.\n");
                continue;
            }
            // Lancement de la partie
            playGame(game);
            // Libération des ressources en fin de partie
            freeGame(game);
        } else if (choix== 2) {
            // Chargement d'une partie sauvegardée
            Game *game = loadGame("save.dat");
            if (!game) {
                printf("Aucune sauvegarde à charger ou échec du chargement.\n");
            } else {
                printf("Partie chargée avec succès.\n");
                playGame(game);
                freeGame(game);
            }
        } else if (choix== 3) {
            // Quitter le programme
            printf("Au revoir!\n");
            break;
        } else {
            printf("Choix invalide. Veuillez réessayer.\n");
        }
    }
    return 0;
}
