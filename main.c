#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "save.h"

int main() {
    printf("=== Bienvenue dans Card Yard ===\n");
    while (1) {
        printf("\nMenu principal:\n");
        printf("1. Nouvelle partie\n");
        printf("2. Charger partie\n");
        printf("3. Quitter\n");
        printf("Votre choix: ");
        int choice = 0;
        if (scanf("%d", &choice) != 1) {
            // Si l'entrÃ©e n'est pas un entier (EOF par ex.), on quitte
            break;
        }
        // Vider le tampon jusqu'au prochain saut de ligne
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        if (choice == 1) {
            // Nouvelle partie
            int numPlayers = 0;
            int cardsPerPlayer = 0;
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
            while (cardsPerPlayer < 1) {
                printf("Entrez le nombre de cartes personnelles par joueur: ");
                if (scanf("%d", &cardsPerPlayer) != 1) {
                    cardsPerPlayer = 0;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    continue;
                }
                while ((ch = getchar()) != '\n' && ch != EOF) {}
                if (cardsPerPlayer < 1) {
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
            // CrÃ©ation du jeu
            Game *game = createGame(numPlayers, cardsPerPlayer, 
                                    (fileChoice=='o'||fileChoice=='O') ? filename : NULL);
            if (!game) {
                printf("Impossible de crÃ©er la partie.\n");
                continue;
            }
            // Lancement de la partie
            playGame(game);
            // LibÃ©ration des ressources en fin de partie
            freeGame(game);
        } else if (choice == 2) {
            // Chargement d'une partie sauvegardÃ©e
            Game *game = loadGame("save.dat");
            if (!game) {
                printf("Aucune sauvegarde Ã  charger ou Ã©chec du chargement.\n");
            } else {
                printf("Partie chargÃ©e avec succÃ¨s.\n");
                playGame(game);
                freeGame(game);
            }
        } else if (choice == 3) {
            // Quitter le programme
            printf("Au revoir!\n");
            break;
        } else {
            printf("Choix invalide. Veuillez rÃ©essayer.\n");
        }
    }
    return 0;
}
