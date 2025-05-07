#ifndef DISPLAY_H
#define DISPLAY_H

#include "jeu.h"

// Affiche le menu principal et retourne le choix de l'utilisateur
int afficherMenu();

// Affiche l'état actuel du jeu (pioche, défausses, cartes des joueurs)
void afficherJeu(const Jeu *jeu, int joueurActuel);

// Affiche une carte (valeur si visible, ou XX si cachée)
void afficherCarte(const Carte *carte, bool visible);

#endif
