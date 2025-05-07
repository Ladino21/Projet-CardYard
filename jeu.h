#ifndef JEU_H
#define JEU_H

#include "cartes.h"
#include "joueurs.h"

// Structure représentant l'état d'une partie de jeu
typedef struct Jeu {
    Carte *pioche;
    int nb_cartes_pioche;
    Joueur *joueurs;
    int nb_joueurs;
} Jeu;

// Initialisation d'une nouvelle partie
void initialiserJeu(Jeu *jeu, int nbJoueurs);

// Boucle de jeu principal (renvoie 0 si fin normale, 1 si sauvegardé et interrompu)
int jouerPartie(Jeu *jeu);

#endif
