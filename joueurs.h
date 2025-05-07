#ifndef JOUEURS_H
#define JOUEURS_H

#include "cartes.h"

#define MAX_PLAYERS 8  // nombre maximal de joueurs

// Structure reprÃ©sentant un joueur
typedef struct {
    Card *personal;     // tableau de cartes personnelles (main/tapis du joueur)
    int personalCount;  // nombre de cartes personnelles
    Card *discard;      // tableau reprÃ©sentant la pile de dÃ©fausse du joueur
    int discardCount;   // nombre de cartes dans la dÃ©fausse (le sommet est discard[discardCount-1])
} Player;

// CrÃ©e les joueurs et distribue les cartes personnelles Ã  chacun depuis la pioche
// numPlayers : nombre de joueurs, cardsPerPlayer : nombre de cartes personnelles par joueur
// deck : pointeur vers le Deck Ã  partir duquel on pioche
// totalCards : nombre total de cartes initiales (utilisÃ© pour dimensionner les piles de dÃ©fausse)
Player* createPlayers(int numPlayers, int cardsPerPlayer, Deck *deck, int totalCards);

// LibÃ¨re le tableau de joueurs et les allocations associÃ©es
void freePlayers(Player *players, int numPlayers);

#endif // JOUEURS_H
