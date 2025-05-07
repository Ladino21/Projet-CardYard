#ifndef SAVE_H
#define SAVE_H

#include "jeu.h"

// Sauvegarde l'état actuel de la partie dans un fichier binaire (retourne 0 si succès, !=0 si échec)
int saveGame(const char *filename, const Game *game);

// Charge une partie depuis un fichier binaire de sauvegarde (renvoie un pointeur vers Game alloué, ou NULL en cas d'échec)
Game* loadGame(const char *filename);

#endif // SAVE_H
