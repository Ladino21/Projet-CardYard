#ifndef JEU_H
#define JEU_H

#include "cartes.h"
#include "joueurs.h"

typedef struct {
    int nb_joueurs;
    int nb_cartes_personnelles;
    int joueur_courant; // indice du joueur dont c'est le tour
    Pioche pioche;
    Joueur *joueurs;
} Partie;


Partie* creerPartie(int nb_joueurs, int nb_cartes_par_joueur, const char *fichier_pioche);

// Lance la boucle de jeu interactive
void jouerPartie(Partie *partie);


void libererPartie(Partie *partie);

#endif 
