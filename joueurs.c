#include <stdio.h>
#include <stdlib.h>
#include "joueurs.h"

Player* creerJoueurs(int numPlayers, int cardsPerPlayer, Deck *deck, int totalCards) {
    if (numPlayers < 2 || numPlayers > MAX_PLAYERS) {
        fprintf(stderr, "Nombre de joueurs invalide: %d. Doit être entre 2 et %d.\n", numPlayers, MAX_PLAYERS);
        return NULL;
    }
    // Allouer le tableau de joueurs
    Player *players = malloc(numPlayers * sizeof(Player));
    if (!players) {
        fprintf(stderr, "Échec d'allocation pour les joueurs\n");
        return NULL;
    }
    for (int i = 0; i < numPlayers; ++i) {
        players[i].personalCount = cardsPerPlayer;
        // Allouer le tableau de cartes personnelles
        players[i].personal = malloc(cardsPerPlayer * sizeof(Card));
        if (!players[i].personal) {
            fprintf(stderr, "Échec d'allocation pour les cartes personnelles du joueur %d\n", i);
            // Libérer ce qui a été alloué pour les joueurs précédents
            for (int k = 0; k < i; ++k) {
                free(players[k].personal);
            }
            free(players);
            return NULL;
        }
        // Distribuer les cartes personnelles en piochant du deck
        for (int c = 0; c < cardsPerPlayer; ++c) {
            Card card = piocherCarte(deck);
            card.visible = false; // les cartes personnelles commencent face cachée
            players[i].personal[c] = card;
        }
        // Allouer le tableau de défausse (capacité = totalCards initial)
        if (totalCards <= 0) totalCards = 1;
        players[i].discard = malloc(totalCards * sizeof(Card));
        if (!players[i].discard) {
            fprintf(stderr, "Échec d'allocation pour la défausse du joueur %d\n", i);
            // Libérer ce joueur et les précédents
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

void libererJoueurs(Player *players, int numPlayers) {
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
