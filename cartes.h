#ifndef CARTES_H
#define CARTES_H

#include <stdbool.h>

// Définition des constantes du jeu
#define VALEUR_MIN       -2    // plus petite valeur de carte
#define VALEUR_MAX        12   // plus grande valeur de carte
#define NB_CARTES_NEG2     5   // nombre de cartes de valeur -2
#define NB_CARTES_NEG1    10   // nombre de cartes de valeur -1
#define NB_CARTES_ZERO    15   // nombre de cartes de valeur  0
#define NB_CARTES_POS     10   // nombre de cartes pour chaque valeur positive
#define NB_CARTES_TOTAL  (NB_CARTES_NEG2 + NB_CARTES_NEG1 + NB_CARTES_ZERO + (VALEUR_MAX * NB_CARTES_POS))

// Structure représentant une carte (valeur + état visible ou non)
typedef struct {
    int valeur;
    bool visible;
} Carte;

// Structure représentant la pioche (tableau statique de cartes + nombre actuel)
typedef struct {
    Carte cartes[NB_CARTES_TOTAL];
    int nb_cartes;  // nombre de cartes restantes dans la pioche
} Pioche;

// Prototypes des fonctions sur la pioche
void generer_pioche(Pioche *p);
void melanger_pioche(Pioche *p);
Carte piocher_carte(Pioche *p);

#endif // CARTES_H
