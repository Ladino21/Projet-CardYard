// === joueurs.h ===
#ifndef JOUEURS_H
#define JOUEURS_H

#include "cartes.h"

#define MAX_JOUEURS 8
#define MAX_NOM 20
#define MAX_CARTES_JOUEUR 20
#define MAX_DEFAUSSE 20

typedef struct {
    char nom[MAX_NOM];
    Carte cartes[MAX_CARTES_JOUEUR];
    int nb_cartes;
    Carte defausse[MAX_DEFAUSSE];
    int nb_defausse;
} Joueur;

void initialiser_joueurs(Joueur *joueurs, int nb_joueurs, int nb_cartes, Carte *pioche, int *nb_pioche);
void afficher_joueur(const Joueur *j);
void ajouter_defausse(Joueur *j, Carte c);
int calculer_score(Joueur *j);
int toutes_visibles(Joueur *j);

#endif
