// cartes.h
#ifndef CARTES_H
#define CARTES_H

#include <stdbool.h>

#define MAX_CARTES 500  // Nombre maximal de cartes

// Structure représentant une carte
typedef struct {
    int valeur;
    bool visible;
} Carte;

// Structure représentant une pioche (deck)
typedef struct {
    Carte *cartes; // Tableau dynamique de cartes
    int taille;    // Nombre de cartes dans la pioche
} Pioche;

// Crée une pioche par défaut
Pioche creerPiocheDefaut();

// Crée une pioche depuis un fichier (valeur:quantite, arrêt sur 0:0)
Pioche creerPiocheDepuisFichier(const char *nomFichier);

// Mélange la pioche
void melangerPioche(Pioche *pioche);

// Pioche une carte du dessus
Carte piocherCarte(Pioche *pioche);

// Libère la mémoire de la pioche
void libererPioche(Pioche *pioche);

#endif
