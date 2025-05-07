#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"
#include "cartes.h"
#include "joueurs.h"

void displayCard(const Card *card) {
    // Affiche une carte unique avec un encadrement
    if (!card) {
        printf("    ");
        return;
    }
    // Largeur intÃ©rieure de la carte (pour afficher la valeur)
    const int width = 2;
    char content[8];
    if (!card->isVisible) {
        // Carte cachÃ©e
        snprintf(content, sizeof(content), "%*s", width, "??");
    } else {
        if (card->value < 0 || card->value > 99) {
            snprintf(content, sizeof(content), "%*s", width, "??");
        } else {
            snprintf(content, sizeof(content), "%*d", width, card->value);
        }
    }
    // Bord supÃ©rieur
    printf("â•”");
    for (int i = 0; i < width; ++i) printf("â•");
    printf("â•—");
    // Pas de saut de ligne ici (gÃ©rÃ© Ã  l'extÃ©rieur pour aligner plusieurs cartes)
}

// Affiche plusieurs cartes cÃ´te Ã  cÃ´te (par exemple la main d'un joueur)
void displayCardsLine(const Card *cards, int count) {
    if (count <= 0) {
        return;
    }
    const int width = 2;
    // Ligne de bord supÃ©rieur pour toutes les cartes
    for (int j = 0; j < count; ++j) {
        printf("â•”");
        for (int i = 0; i < width; ++i) printf("â•");
        printf("â•—");
        if (j < count - 1) printf(" ");
    }
    printf("\n");
    // Ligne centrale avec les valeurs
    for (int j = 0; j < count; ++j) {
        const Card *card = &cards[j];
        printf("â•‘");
        if (!card->isVisible) {
            // carte cachÃ©e
            printf("??");
        } else {
            if (card->value < 0 || card->value > 99) {
                printf("??");
            } else {
                // Alignement Ã  droite sur 2 caractÃ¨res pour la valeur
                if (card->value < 10) {
                    printf(" %d", card->value);
                } else {
                    printf("%2d", card->value);
                }
            }
        }
        printf("â•‘");
        if (j < count - 1) printf(" ");
    }
    printf("\n");
    // Ligne de bord infÃ©rieur pour toutes les cartes
    for (int j = 0; j < count; ++j) {
        printf("â•š");
        for (int i = 0; i < 2; ++i) printf("â•");
        printf("â•");
        if (j < count - 1) printf(" ");
    }
    printf("\n");
}

// Affiche l'Ã©tat du jeu (les cartes de chaque joueur et le sommet de leur dÃ©fausse)
void displayGame(const Game *game) {
    if (!game) return;
    for (int p = 0; p < game->numPlayers; ++p) {
        printf("Joueur %d :\n", p+1);
        // Afficher les cartes personnelles du joueur p
        displayCardsLine(game->players[p].personal, game->players[p].personalCount);
        // Afficher les indices des cartes en dessous pour repÃ¨re
        if (game->players[p].personalCount > 0) {
            int cardWidth = 4;   // largeur totale d'une carte (2 intÃ©rieurs + 2 bordures)
            int spacing = 1;    // espace entre les cartes affichÃ©es
            for (int j = 0; j < game->players[p].personalCount; ++j) {
                int offset = j * (cardWidth + spacing);
                for (int k = 0; k < offset + 1; ++k) printf(" ");
                // Afficher l'index du j-iÃ¨me emplacement
                printf("%d", j);
            }
            printf("\n");
        }
        // Afficher le sommet de la dÃ©fausse du joueur p
        printf("DÃ©fausse: ");
        if (game->players[p].discardCount > 0) {
            Card topCard = game->players[p].discard[ game->players[p].discardCount - 1 ];
            // On affiche seulement la carte du dessus de la dÃ©fausse (toujours visible)
            displayCardsLine(&topCard, 1);
        } else {
            printf("(vide)\n");
        }
        printf("\n");
    }
}
