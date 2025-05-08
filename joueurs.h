#ifndef JOUEURS_H
#define JOUEURS_H

#include "cartes.h"

#define MAX_PLAYERS 8  // nombre maximal de joueurs

// Structure représentant un joueur
typedef struct {
    Card *personal;     // tableau de cartes personnelles (main/tapis du joueur)
    int comptePersonnel;  // nombre de cartes personnelles
    Card *discard;      // tableau représentant la pile de défausse du joueur
    int discardCount;   // nombre de cartes dans la défausse (le sommet est discard[discardCount-1])
} Player;

// Crée les joueurs et distribue les cartes personnelles à chacun depuis la pioche
// numPlayers : nombre de joueurs, cardsPerPlayer : nombre de cartes personnelles par joueur
// deck : pointeur vers le Deck à partir duquel on pioche
// totalCards : nombre total de cartes initiales (utilisé pour dimensionner les piles de défausse)
Player* creerJoueurs(int numPlayers, int cartesParJoueur, Deck *deck, int totalCards);

// Libère le tableau de joueurs et les allocations associées
void libererJoueurs(Player *players, int numPlayers);

#endif // JOUEURS_H
