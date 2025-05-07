#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"
#include "cartes.h"
#include "joueurs.h"

void displayCardsLine(const Card *cards, int count) {
    if (count <= 0) return;
    const int width = 4;

    // Bord supérieur
    for (int j = 0; j < count; j++) {
        printf("+------+" );
        if (j < count - 1) printf(" ");
    }
    printf("\n");

    // Ligne avec les valeurs
    for (int j = 0; j < count; j++) {
        const Card *card = &cards[j];
        printf("| ");
        if (!card->visible) {
            printf(" ?? ");
        } else {
            if (card->valeur < 0 || card->valeur > 99) {
                printf(" ?? ");
            } else {
                printf("%2d ", card->valeur);
            }
        }
        printf("|");
        if (j < count - 1) printf(" ");
    }
    printf("\n");

    // Bord inférieur
    for (int j = 0; j < count; j++) {
        printf("+------+" );
        if (j < count - 1) printf(" ");
    }
    printf("\n");
}

void displayGame(const Game *game) {
    if (!game) return;
    for (int p = 0; p < game->numPlayers; ++p) {
        printf("Joueur %d :\n", p + 1);
        displayCardsLine(game->players[p].personal, game->players[p].personalCount);

        // Indices en dessous des cartes
        if (game->players[p].personalCount > 0) {
            for (int j = 0; j < game->players[p].personalCount; ++j) {
                printf("   %d   ", j);
                if (j < game->players[p].personalCount - 1) printf(" ");
            }
            printf("\n");
        }

        // Défausse
        printf("Défausse : ");
        if (game->players[p].discardCount > 0) {
            Card topCard = game->players[p].discard[game->players[p].discardCount - 1];
            displayCardsLine(&topCard, 1);
        } else {
            printf("(vide)\n");
        }

        printf("\n");
    }
}
