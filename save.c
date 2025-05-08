#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "cartes.h"
#include "joueurs.h"

int saveGame(const char *filename, const Game *game) {
    if (!game || !filename) return -1;
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }
    // Écrire les paramètres de base du jeu
    fwrite(&game->numPlayers, sizeof(int), 1, f);
    fwrite(&game->cartesParJoueur, sizeof(int), 1, f);
    fwrite(&game->joueurActuel, sizeof(int), 1, f);
    // Écrire la pioche (taille et cartes restantes)
    fwrite(&game->deck.size, sizeof(int), 1, f);
    if (game->deck.size > 0) {
        fwrite(game->deck.cards, sizeof(Card), game->deck.size, f);
    }
    // Écrire les données de chaque joueur
    for (int i = 0; i < game->numPlayers; ++i) {
        // Par sécurité, on écrit personalCount (normalement identique pour tous les joueurs)
        fwrite(&game->players[i].personalCount, sizeof(int), 1, f);
        if (game->players[i].personalCount > 0) {
            fwrite(game->players[i].personal, sizeof(Card), game->players[i].personalCount, f);
        }
        fwrite(&game->players[i].discardCount, sizeof(int), 1, f);
        if (game->players[i].discardCount > 0) {
            fwrite(game->players[i].discard, sizeof(Card), game->players[i].discardCount, f);
        }
    }
    fclose(f);
    return 0;
}

Game* loadGame(const char *filename) {
    if (!filename) return NULL;
    FILE *f = fopen(filename, "rb");
    if (!f) {
        return NULL;
    }
    // Allouer une nouvelle structure Game
    Game *game = malloc(sizeof(Game));
    if (!game) {
        fclose(f);
        return NULL;
    }
    if (fread(&game->numPlayers, sizeof(int), 1, f) != 1) {
        fclose(f);
        free(game);
        return NULL;
    }
    fread(&game->cartesParJoueur, sizeof(int), 1, f);
    fread(&game->joueurActuel, sizeof(int), 1, f);
    // Lire la pioche
    int deckSize = 0;
    fread(&deckSize, sizeof(int), 1, f);
    game->deck.size = deckSize;
    if (deckSize > 0) {
        game->deck.cards = malloc(deckSize * sizeof(Card));
        if (!game->deck.cards) {
            fclose(f);
            free(game);
            return NULL;
        }
        fread(game->deck.cards, sizeof(Card), deckSize, f);
    } else {
        game->deck.cards = NULL;
    }
    // Allouer le tableau de joueurs
    game->players = malloc(game->numPlayers * sizeof(Player));
    if (!game->players) {
        if (game->deck.cards) free(game->deck.cards);
        free(game);
        fclose(f);
        return NULL;
    }
    // Calculer le nombre total initial de cartes pour dimensionner les défausses
    // total initial = deckSize + total cartes personnelles + total cartes en défausse
    int totalPersonal = game->numPlayers * game->cartesParJoueur;
    int totalDiscard = 0;
    // Sauvegarder la position du fichier avant de lire les détails des joueurs
    fpos_t pos;
    fgetpos(f, &pos);
    // Parcourir les données joueurs pour calculer totalDiscard
    for (int i = 0; i < game->numPlayers; ++i) {
        int pCount;
        int dCount;
        fread(&pCount, sizeof(int), 1, f);
        fseek(f, pCount * sizeof(Card), SEEK_CUR);
        fread(&dCount, sizeof(int), 1, f);
        totalDiscard += dCount;
        fseek(f, dCount * sizeof(Card), SEEK_CUR);
    }
    // Revenir au début des données joueurs
    fsetpos(f, &pos);
    int initialTotal = deckSize + totalPersonal + totalDiscard;
    if (initialTotal < 1) initialTotal = 1;
    // Lire réellement les données joueurs en allouant les mémoires nécessaires
    for (int i = 0; i < game->numPlayers; ++i) {
        // Lire et allouer les cartes personnelles du joueur
        int personalCount;
        fread(&personalCount, sizeof(int), 1, f);
        game->players[i].personalCount = personalCount;
        game->players[i].personal = malloc(personalCount * sizeof(Card));
        if (!game->players[i].personal) {
            // En cas d'échec, libérer ce qui a été alloué précédemment et quitter
            for (int k = 0; k < i; ++k) {
                free(game->players[k].personal);
                free(game->players[k].discard);
            }
            free(game->players);
            if (game->deck.cards) free(game->deck.cards);
            free(game);
            fclose(f);
            return NULL;
        }
        if (personalCount > 0) {
            fread(game->players[i].personal, sizeof(Card), personalCount, f);
        }
        // Lire et allouer la pile de défausse du joueur
        fread(&game->players[i].discardCount, sizeof(int), 1, f);
        int dCount = game->players[i].discardCount;
        game->players[i].discard = malloc(initialTotal * sizeof(Card));
        if (!game->players[i].discard) {
            // En cas d'échec, libérer les ressources allouées et quitter
            free(game->players[i].personal);
            for (int k = 0; k < i; ++k) {
                free(game->players[k].personal);
                free(game->players[k].discard);
            }
            free(game->players);
            if (game->deck.cards) free(game->deck.cards);
            free(game);
            fclose(f);
            return NULL;
        }
        if (dCount > 0) {
            fread(game->players[i].discard, sizeof(Card), dCount, f);
        }
    }
    fclose(f);
    return game;
}
