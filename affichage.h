#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "cartes.h"
#include "joueurs.h"
#include "jeu.h"

// Couleurs et mise en forme
const char* couleurCarte(int valeur);

// Affichages de cartes
void afficherCarteStylisee(const Carte *carte);
void afficherLigneCartes(const Carte *cartes, int nbCartes);
void afficherEspacesCentrage(int nbCartes);
void afficherClassement(const Partie *partie);

// Affichages de zones
void afficherPiocheCentrale(const Pioche *pioche);
void afficherDefausse(const Joueur *joueur, int nbCartes);
void afficherPartie(const Partie *partie);

#endif // AFFICHAGE_H
