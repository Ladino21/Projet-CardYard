// === save.h ===
#ifndef SAVE_H
#define SAVE_H

#include "cartes.h"
#include "joueurs.h"

int sauvegarder_partie(const char *fichier, Joueur *joueurs, int nb_joueurs, Carte *pioche, int nb_pioche);
int charger_partie(const char *fichier, Joueur *joueurs, int *nb_joueurs, int *nb_cartes, Carte *pioche, int *nb_pioche);

#endif
