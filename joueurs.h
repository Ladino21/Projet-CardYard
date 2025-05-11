#ifndef JOUEURS_H
#define JOUEURS_H

#include "cartes.h"

#define NB_JOUEURS_MAX 8  // nombre maximal de joueurs

// Structure représentant un joueur
typedef struct {
    Carte *personnelles;     
    int nb_cartes;           
    Carte *defausse;         
    int nb_defausse;         
} Joueur;

// Crée les joueurs et leur distribue les cartes depuis la pioche
// nb_joueurs : nombre total de joueurs
// nb_cartes_par_joueur : nombre de cartes personnelles par joueur
// pioche : pointeur vers la pioche
// nb_cartes_total : taille initiale de la pioche (sert à dimensionner les défausses)
Joueur* creerJoueurs(int nb_joueurs, int nb_cartes_par_joueur, Pioche *pioche, int nb_cartes_total);

// Libère la mémoire allouée pour les joueurs et leurs cartes
void libererJoueurs(Joueur *joueurs, int nb_joueurs);

#endif // JOUEURS_H
