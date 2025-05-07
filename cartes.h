#ifndef CARTES_H
#define CARTES_H

#include <stdbool.h>

#define MAX_CARTES 500  // Limite maximale de cartes dans une pioche

// Structure représentant une carte
typedef struct {
    int valeur;          // Valeur de la carte (-2 à 12 en général)
    bool estCachee;      // true = face cachée, false = visible
} Carte;

// Structure représentant une pioche
typedef struct {
    Carte cartes[MAX_CARTES];  // Tableau de cartes (fixe)
    int nombreCartes;          // Nombre de cartes actuellement dans la pioche
} Pioche;

// Crée une pioche avec les valeurs par défaut (-2 à 12)
Pioche *creerPiocheDefaut(void);

// Crée une pioche à partir d’un fichier texte ("valeur:quantité" par ligne)
// Arrêt à la ligne "0:0" ou si erreur
Pioche *creerPiocheDepuisFichier(const char *nomFichier);

// Mélange les cartes de la pioche
void melangerPioche(Pioche *pioche);

// Pioche une carte (retire la dernière carte de la pioche)
Carte piocherCarte(Pioche *pioche);

// Libère la mémoire allouée pour une pioche
void libererPioche(Pioche *pioche);

#endif // CARTES_H
