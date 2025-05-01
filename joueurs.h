// joueurs.h
#ifndef CARTES_H
#define CARTES_H

#include "cartes.h"

#define MAX_CARTES 20  // nombre maximum de cartes personnelles
#define MAX_DEFAUSSE 20 // nombre maximum de cartes dans la défausse

typedef struct {
    char nom[50];
    Carte cartes[MAX_CARTES];   // cartes personnelles
    int nb_cartes;              // nombre effectif de cartes dans la main
    Carte defausse[MAX_DEFAUSSE]; // défausse (cartes échangées)
    int nb_defausse;
} Joueur;

void initialiser_joueurs(Joueur *joueurs, int nb_joueurs, int nb_cartes_par_joueur);
void afficher_joueur(const Joueur *joueur);
void reveler_joueur(Joueur *joueur);
int calculer_score(const Joueur *joueur);
#endif
