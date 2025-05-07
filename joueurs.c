#include <stdio.h>
#include <stdlib.h>
#include "joueurs.h"

Player* createPlayers(int numPlayers, int cardsPerPlayer, Deck *deck, int totalCards) {
    if (numPlayers < 2 || numPlayers > MAX_PLAYERS) {
        fprintf(stderr, "Nombre de joueurs invalide: %d. Doit Ãªtre entre 2 et %d.\n", numPlayers, MAX_PLAYERS);
        return NULL;
    }
    // Allouer le tableau de joueurs
    Player *players = malloc(numPlayers * sizeof(Player));
    if (!players) {
        fprintf(stderr, "Ã‰chec d'allocation pour les joueurs\n");
        return NULL;
    }
    for (int i = 0; i < numPlayers; ++i) {
        players[i].personalCount = cardsPerPlayer;
        // Allouer le tableau de cartes personnelles
        players[i].personal = malloc(cardsPerPlayer * sizeof(Card));
        if (!players[i].personal) {
            fprintf(stderr, "Ã‰chec d'allocation pour les cartes personnelles du joueur %d\n", i);
            // LibÃ©rer ce qui a Ã©tÃ© allouÃ© pour les joueurs prÃ©cÃ©dents
            for (int k = 0; k < i; ++k) {
                free(players[k].personal);
            }
            free(players);
            return NULL;
        }
        // Distribuer les cartes personnelles en piochant du deck
        for (int c = 0; c < cardsPerPlayer; ++c) {
            Card card = drawCard(deck);
            card.isVisible = false; // les cartes personnelles commencent face cachÃ©e
            players[i].personal[c] = card;
        }
        // Allouer le tableau de dÃ©fausse (capacitÃ© = totalCards initial)
        if (totalCards <= 0) totalCards = 1;
        players[i].discard = malloc(totalCards * sizeof(Card));
        if (!players[i].discard) {
            fprintf(stderr, "Ã‰chec d'allocation pour la dÃ©fausse du joueur %d\n", i);
            // LibÃ©rer ce joueur et les prÃ©cÃ©dents
            free(players[i].personal);
            for (int k = 0; k < i; ++k) {
                free(players[k].personal);
                free(players[k].discard);
            }
            free(players);
            return NULL;
        }
        players[i].discardCount = 0;
    }
    return players;
}

void freePlayers(Player *players, int numPlayers) {
    if (!players) return;
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i].personal) {
            free(players[i].personal);
            players[i].personal = NULL;
        }
        if (players[i].discard) {
            free(players[i].discard);
            players[i].discard = NULL;
        }
    }
    free(players);
}
