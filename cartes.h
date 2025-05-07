#ifndef CARTES_H
#define CARTES_H

#include <stdbool.h>

// DÃ©finir un maximum de cartes pour des allocations sÃ»res (ajustable si besoin)
#define MAX_CARDS 500

// Structure reprÃ©sentant une carte
typedef struct {
    int value;
    bool isVisible;
} Card;

// Structure reprÃ©sentant la pioche (deck de cartes Ã  piocher)
typedef struct {
    Card *cards;  // tableau dynamique de cartes
    int size;     // nombre de cartes actuellement dans la pioche
} Deck;

// CrÃ©e une pioche par dÃ©faut (jeu de cartes valeurs par dÃ©faut)
Deck createDeckDefault();

// CrÃ©e une pioche en chargeant les cartes depuis un fichier "valeur:quantite"
Deck createDeckFromFile(const char *filename);

// MÃ©lange la pioche
void shuffleDeck(Deck *deck);

// Pioche une carte du deck (retire la carte du haut de la pioche et la renvoie)
Card drawCard(Deck *deck);

// LibÃ¨re la mÃ©moire allouÃ©e pour le deck
void freeDeck(Deck *deck);

#endif // CARTES_H
