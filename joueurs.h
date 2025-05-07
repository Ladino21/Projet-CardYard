#ifndef JOUEURS_H
#define JOUEURS_H

#include "cartes.h"
#include <stdbool.h>

#define NB_CARTES_PERSONNELLES 6

// Structure représentant un joueur et ses cartes
typedef struct Joueur {
    Carte cartes_personnelles[NB_CARTES_PERSONNELLES];
    bool cartes_visibles[NB_CARTES_PERSONNELLES];
    Carte *defausse;
    int nb_defausse;
    int capacite_defausse;
} Joueur;

// Fonctions liées aux joueurs
Joueur* creerJoueurs(int nbJoueurs);
void libererJoueurs(Joueur *joueurs, int nbJoueurs);
int calculerScore(Joueur *joueur);
void ajouterCarteDefausse(Joueur *joueur, Carte carte);

#endif
