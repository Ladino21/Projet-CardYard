#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jeu.h"
#include "cartes.h"
#include "joueurs.h"
#include "save.h"

// Déclarations anticipées des fonctions d'affichage (implémentées dans display.c)
void displayGame(const Game *game);
void displayPlayer(const Player *player, int index);
void displayCard(const Card *card);

// Fonction auxiliaire pour demander un entier dans une plage [min, max]
int promptInt(const char *message, int min, int max) {
    int value;
    char line[64];
    while (1) {
        printf("%s", message);
        if (!fgets(line, sizeof(line), stdin)) {
            // En cas d'erreur de saisie ou EOF
            return -1;
        }
        // Retirer le saut de ligne
        line[strcspn(line, "\n")] = '\0';
        if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Veuillez entrer un nombre entre %d et %d.\n", min, max);
    }
}

Game* createGame(int numPlayers, int cardsPerPlayer, const char *deckFile) {
    // Allouer la structure Game
    Game *game = malloc(sizeof(Game));
    if (!game) {
        fprintf(stderr, "Erreur d'allocation pour la structure Game\n");
        return NULL;
    }
    game->numPlayers = numPlayers;
    game->cardsPerPlayer = cardsPerPlayer;
    game->currentPlayer = 0;
    // Créer la pioche (depuis le fichier si fourni, sinon pioche par défaut)
    if (deckFile && strlen(deckFile) > 0) {
        game->deck = createDeckFromFile(deckFile);
    } else {
        game->deck = createDeckDefault();
    }
    // Vérifier qu'il y a assez de cartes pour distribuer les cartes personnelles
    if (numPlayers * cardsPerPlayer > game->deck.size) {
        printf("La pioche ne contient que %d cartes, insuffisant pour %d joueurs avec %d cartes chacun.\n",
               game->deck.size, numPlayers, cardsPerPlayer);
        printf("Veuillez réduire le nombre de joueurs ou de cartes par joueur.\n");
        free(game);
        return NULL;
    }
    // Nombre total de cartes initiales (avant distribution) pour dimensionner les défausses
    int totalCards = game->deck.size;
    // Créer les joueurs et distribuer leurs cartes personnelles
    game->players = createPlayers(numPlayers, cardsPerPlayer, &game->deck, totalCards);
    if (!game->players) {
        freeDeck(&game->deck);
        free(game);
        return NULL;
    }
    return game;
}

void playGame(Game *game) {
    if (!game) return;
    char input[64];
    int gameOver = 0;
    // Boucle principale du jeu
    while (!gameOver) {
        printf("\n===== Tour du joueur %d =====\n", game->currentPlayer + 1);
        // Afficher l'état courant du jeu (mains et défausses)
        displayGame(game);
        // Proposer les actions au joueur courant
        printf("Joueur %d, choisissez une action:\n", game->currentPlayer + 1);
        printf("[0] Piocher dans la pioche centrale\n");
        printf("[1-%d] Piocher dans la défausse d'un joueur (entrez le numéro du joueur)\n", game->numPlayers);
        printf("[S] Sauvegarder la partie\n");
        printf("[Q] Quitter\n");
        printf("Votre choix: ");
        if (!fgets(input, sizeof(input), stdin)) {
            // Gestion d'EOF éventuel
            return;
        }
        // Retirer le saut de ligne
        input[strcspn(input, "\n")] = '\0';
        // Gérer les commandes spéciales par lettre (Q, S) si l'utilisateur a entré un seul caractère
        if (strlen(input) == 1) {
            char ch = toupper(input[0]);
            if (ch == 'Q') {
                // Quitter sans sauvegarder
                printf("Quitter la partie sans sauvegarder? (o/n): ");
                char confirm;
                if (scanf(" %c", &confirm) == 1) {
                    if (tolower(confirm) == 'o') {
                        printf("Partie terminée.\n");
                        return;
                    }
                }
                // Vider le tampon après la saisie confirm/annulation
                int c;
                while((c = getchar()) != '\n' && c != EOF) {}
                continue; // revenir à la boucle de jeu
            }
            if (ch == 'S') {
                // Sauvegarder la partie
                if (saveGame("save.dat", game) == 0) {
                    printf("Jeu sauvegardé dans save.dat.\n");
                } else {
                    printf("Erreur lors de la sauvegarde de la partie.\n");
                }
                // Après la sauvegarde, demander si on continue ou si on quitte
                printf("Continuer la partie? (o/n): ");
                char resp;
                if (scanf(" %c", &resp) == 1) {
                    if (tolower(resp) != 'o') {
                        printf("Partie sauvegardée et terminée.\n");
                        return;
                    }
                }
                // Vider le tampon
                int c;
                while((c = getchar()) != '\n' && c != EOF) {}
                continue; // reprendre la boucle du jeu
            }
        }
        // A ce stade, on traite l'entrée comme un choix numérique
        int choice = atoi(input);
        Card drawnCard;
        int fromDiscardPlayer = -1;
        if (choice == 0) {
            // Piocher dans la pioche centrale
            if (game->deck.size == 0) {
                printf("La pioche centrale est vide! Choisissez une autre action.\n");
                continue;
            }
            drawnCard = drawCard(&game->deck);
            // Afficher la valeur de la carte piochée au joueur courant
            printf("Vous avez pioché la carte de valeur %d.\n", drawnCard.valeur);
        } else if (choice >= 1 && choice <= game->numPlayers) {
            int targetPlayer = choice - 1;
            if (game->players[targetPlayer].discardCount == 0) {
                printf("La défausse du joueur %d est vide. Choisissez une autre action.\n", targetPlayer + 1);
                continue;
            }
            // Prendre la carte du dessus de la défausse du joueur désigné
            drawnCard = game->players[targetPlayer].discard[ game->players[targetPlayer].discardCount - 1 ];
            game->players[targetPlayer].discardCount--;
            fromDiscardPlayer = targetPlayer;
            printf("Vous prenez la carte %d de la défausse du joueur %d.\n", drawnCard.valeur, targetPlayer + 1);
        } else {
            printf("Choix invalide. Veuillez réessayer.\n");
            continue;
        }
        // On a maintenant une carte dans drawnCard (depuis la pioche ou une défausse)
        drawnCard.visible = true; // La carte piochée est connue du joueur courant (visible si ajoutée à sa main)
        // Le joueur courant décide s'il échange cette carte ou la défausse directement
        int swapIndex = -1;
        if (choice == 0) {
            // Si la carte vient de la pioche, option de ne pas l'échanger
            printf("Voulez-vous échanger cette carte avec l'une de vos cartes personnelles? (o/n): ");
            char rep;
            if (scanf(" %c", &rep) == 1) {
                // consommer la fin de ligne de la saisie
            }
            int c;
            while((c = getchar()) != '\n' && c != EOF) {}
            if (tolower(rep) == 'n') {
                // Choix de ne pas échanger : on défaussera la carte piochée
                swapIndex = -1;
            } else {
                // Choix d'échanger : demander l'index de la carte personnelle à échanger
                swapIndex = promptInt("Entrez le numéro de la carte personnelle à échanger (0 - index max): ",
                                       0, game->players[game->currentPlayer].personalCount - 1);
            }
        } else {
            // Si la carte vient d'une défausse, on suppose que le joueur veut forcément l'utiliser (échanger)
            swapIndex = promptInt("Entrez le numéro de la carte personnelle à échanger avec cette carte: ",
                                   0, game->players[game->currentPlayer].personalCount - 1);
        }
        if (swapIndex >= 0 && swapIndex < game->players[game->currentPlayer].personalCount) {
            // Vérifier que la carte personnelle choisie n'est pas déjà visible (non échangeable)
            if (game->players[game->currentPlayer].personal[swapIndex].visible) {
                printf("Vous ne pouvez pas échanger une carte déjà visible. Tour annulé.\n");
                // Si le joueur a choisi par erreur une carte visible, on annule l'action:
                // Si la carte provenait d'une défausse, il faut la remettre où elle était.
                if (fromDiscardPlayer != -1) {
                    // Remettre la carte dans la défausse d'origine
                    game->players[fromDiscardPlayer].discard[ game->players[fromDiscardPlayer].discardCount ] = drawnCard;
                    game->players[fromDiscardPlayer].discardCount++;
                } else {
                    // Si elle venait de la pioche, remettre la carte en haut de la pioche
                    game->deck.cards[ game->deck.size ] = drawnCard;
                    game->deck.size++;
                }
                // On passe simplement au prochain tour (la carte n'est pas utilisée)
            } else {
                // Effectuer l'échange
                Card replacedCard = game->players[game->currentPlayer].personal[swapIndex];
                // Placer la carte piochée dans la main du joueur
                game->players[game->currentPlayer].personal[swapIndex] = drawnCard;
                game->players[game->currentPlayer].personal[swapIndex].visible = true;
                // La carte remplacée est défaussée par le joueur courant
                replacedCard.visible = true; // on la révèle au moment de la défausser
                game->players[game->currentPlayer].discard[ game->players[game->currentPlayer].discardCount ] = replacedCard;
                game->players[game->currentPlayer].discardCount++;
                printf("Vous avez échangé la carte de valeur %d avec votre carte de valeur %d.\n",
                       drawnCard.valeur, replacedCard.valeur);
            }
        } else {
            // Si swapIndex == -1 : le joueur défausse la carte piochée sans l'échanger
            game->players[game->currentPlayer].discard[ game->players[game->currentPlayer].discardCount ] = drawnCard;
            game->players[game->currentPlayer].discardCount++;
            printf("Vous avez défaussé la carte de valeur %d sans l'utiliser.\n", drawnCard.valeur);
        }
        // Vérifier la condition de fin de partie :
        // Ici on décide de terminer la partie si un joueur a toutes ses cartes personnelles visibles
        for (int i = 0; i < game->numPlayers; ++i) {
            int visCount = 0;
            for (int j = 0; j < game->players[i].personalCount; ++j) {
                if (game->players[i].personal[j].visible) {
                    visCount++;
                }
            }
            if (visCount == game->players[i].personalCount) {
                // Toutes les cartes du joueur i sont visibles -> fin de la partie
                gameOver = 1;
                printf("Toutes les cartes du joueur %d sont visibles. Fin de la partie.\n", i+1);
                break;
            }
        }
        if (gameOver) {
            break;
        }
        // Passer au joueur suivant
        game->currentPlayer = (game->currentPlayer + 1) % game->numPlayers;
    }
    // La partie est terminée, on peut afficher l'état final (par exemple toutes les cartes)
    printf("État final du jeu:\n");
    displayGame(game);
    printf("Merci d'avoir joué !\n");
}

void freeGame(Game *game) {
    if (!game) return;
    // Libérer les joueurs (leurs mains et défausses)
    if (game->players) {
        freePlayers(game->players, game->numPlayers);
        game->players = NULL;
    }
    // Libérer la pioche
    freeDeck(&game->deck);
    // Libérer la structure Game
    free(game);
}
