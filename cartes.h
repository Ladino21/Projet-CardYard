#ifndef CARTES_H
#define CARTES_H

#include <stdbool.h>

#define NB_CARTES_MAX 150

// Structure représentant une carte
typedef struct {
    int valeur;
    bool visible;
} Carte;

// Structure représentant la pioche (ensemble de cartes à piocher)
typedef struct {
    Carte *cartes;  // tableau dynamique de cartes
    int taille;     // nombre de cartes actuellement dans la pioche
} Pioche;

// Crée une pioche par défaut (jeu de cartes standard)
Pioche creerPiocheDefaut();

// Crée une pioche en chargeant les cartes depuis un fichier "valeur:quantite"
Pioche creerPiocheDepuisFichier(const char *nomFichier);

// Mélange la pioche
void melangerPioche(Pioche *pioche);

// Pioche une carte du dessus de la pioche
Carte piocherCarte(Pioche *pioche);

// Libère la mémoire allouée pour la pioche
void libererPioche(Pioche *pioche);

#endif // CARTES_H
