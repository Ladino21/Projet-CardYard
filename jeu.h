#ifndef JEU_H
#define JEU_H

#include "cartes.h"
#include "joueurs.h"

// Structure représentant l'état global du jeu
typedef struct {
    int numPlayers;
    int cartesParJoueur;
    int currentPlayer; // index (0-based) du joueur dont c'est le tour
    Deck deck;
    Player *players;
} Game;

// Crée une nouvelle partie avec les paramètres donnés (renvoie un pointeur vers Game alloué)
Game* createGame(int numPlayers, int cartesParJoueur, const char *deckFile);

// Lance la boucle de jeu interactive pour l'état de jeu donné
void playGame(Game *game);

// Libère toutes les ressources associées à une partie
void freeGame(Game *game);

#endif // JEU_H
