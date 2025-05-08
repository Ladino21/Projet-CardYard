#ifndef CARTES_H
#define CARTES_H
#include <stdbool.h>
#define MAX_CARDS 150

// Structure représentant une carte
typedef struct {
    int valeur;
    bool visible;
} Card;

// Structure représentant la pioche (deck de cartes à piocher)
typedef struct {
    Card *cards;  // tableau dynamique de cartes
    int size;     // nombre de cartes actuellement dans la pioche
} Deck;

// Crée une pioche par défaut (jeu de cartes valeurs par défaut)
Deck creerPaquetParDefaut();

// Crée une pioche en chargeant les cartes depuis un fichier "valeur:quantite"
Deck creerPaquetFichier(const char *filename);

// Mélange la pioche
void MelangePaquet(Deck *deck);

// Pioche une carte du deck (retire la carte du haut de la pioche et la renvoie)
Card piocherCarte(Deck *deck);

// Libère la mémoire allouée pour le deck
void libererPaquet(Deck *deck);

#endif // CARTES_H
