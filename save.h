#ifndef SAVE_H
#define SAVE_H

#include "jeu.h"

// Sauvegarde l'état actuel de la partie dans un fichier binaire
// Retourne 0 si succès, une valeur différente de 0 en cas d'échec
int sauvegarderPartie(const char *nom_fichier, const Partie *partie);

// Charge une partie depuis un fichier binaire
// Renvoie un pointeur alloué vers Partie, ou NULL si échec
Partie* chargerPartie(const char *nom_fichier);

#endif // SAVE_H
