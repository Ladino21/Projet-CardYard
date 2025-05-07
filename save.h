#ifndef SAVE_H
#define SAVE_H

#include "jeu.h"

// Sauvegarde l'état du jeu dans un fichier (retourne 0 si réussi, 1 en cas d'erreur)
int sauvegarderJeu(const char *nomFichier, const Jeu *jeu);

// Charge l'état du jeu depuis un fichier (retourne 0 si réussi, 1 en cas d'erreur)
int chargerJeu(const char *nomFichier, Jeu *jeu);

#endif
