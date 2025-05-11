#ifndef JOUEURS_H
#define JOUEURS_H

#include "cartes.h"

#define NB_JOUEURS_MAX 8  // nombre maximal de joueurs


typedef struct {
    Carte *personnelles;     
    int nb_cartes;           
    Carte *defausse;         
    int nb_defausse;         
} Joueur;

Joueur* creerJoueurs(int nb_joueurs, int nb_cartes_par_joueur, Pioche *pioche, int nb_cartes_total);

void libererJoueurs(Joueur *joueurs, int nb_joueurs);

#endif 
