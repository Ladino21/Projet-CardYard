#ifndef JEU_H
#define JEU_H

#include "cartes.h"
#include "joueurs.h"

// Structure représentant l'état global de la partie
typedef struct {
    int nb_joueurs;
    int nb_cartes_personnelles;
    int joueur_courant; // indice du joueur dont c'est le tour
    Pioche pioche;
    Joueur *joueurs;
} Partie;

// Crée une nouvelle partie avec les paramètres donnés (renvoie un pointeur alloué)
Partie* creerPartie(int nb_joueurs, int nb_cartes_par_joueur, const char *fichier_pioche);

// Lance la boucle de jeu interactive
void jouerPartie(Partie *partie);

// Libère toutes les ressources associées à une partie
void libererPartie(Partie *partie);

#endif // JEU_H
