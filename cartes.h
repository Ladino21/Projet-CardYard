#ifndef CARTES_H
#define CARTES_H

#include <stdbool.h>

#define NB_CARTES_MAX 150

typedef struct {
    int valeur;
    bool visible;
} Carte;

typedef struct {
    Carte *cartes;  
    int taille;     
} Pioche;


Pioche creerPiocheDefaut();

// Crée une pioche en chargeant les cartes depuis un fichier "valeur:quantite"
Pioche creerPiocheDepuisFichier(const char *nomFichier);


void melangerPioche(Pioche *pioche);


Carte piocherCarte(Pioche *pioche);


void libererPioche(Pioche *pioche);

#endif // CARTES_H
