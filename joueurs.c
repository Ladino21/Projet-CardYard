#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "joueurs.h"

// Crée un tableau de joueurs et initialise leurs structures
Joueur* creerJoueurs(int nbJoueurs) {
    Joueur *joueurs = malloc(nbJoueurs * sizeof(Joueur));
    if (joueurs == NULL) {
        fprintf(stderr, "Erreur allocation mémoire pour les joueurs.\n");
        exit(EXIT_FAILURE);
    }
    for (int j = 0; j < nbJoueurs; ++j) {
        // Initialiser les cartes personnelles comme cachées
        for (int i = 0; i < NB_CARTES_PERSONNELLES; ++i) {
            joueurs[j].cartes_visibles[i] = false;
        }
        // Initialiser la pile de défausse du joueur
        joueurs[j].nb_defausse = 0;
        joueurs[j].capacite_defausse = 1;
        joueurs[j].defausse = malloc(joueurs[j].capacite_defausse * sizeof(Carte));
        if (joueurs[j].defausse == NULL) {
            fprintf(stderr, "Erreur allocation mémoire pour la défausse du joueur %d.\n", j+1);
            exit(EXIT_FAILURE);
        }
    }
    return joueurs;
}

// Libère la mémoire allouée pour les joueurs et leurs défausses
void libererJoueurs(Joueur *joueurs, int nbJoueurs) {
    if (joueurs == NULL) return;
    for (int j = 0; j < nbJoueurs; ++j) {
        free(joueurs[j].defausse);
    }
    free(joueurs);
}

// Calcule le score d'un joueur (somme des valeurs de ses cartes personnelles)
int calculerScore(Joueur *joueur) {
    int score = 0;
    for (int i = 0; i < NB_CARTES_PERSONNELLES; ++i) {
        score += joueur->cartes_personnelles[i].valeur;
    }
    return score;
}

// Ajoute une carte à la défausse du joueur en agrandissant le tableau si nécessaire
void ajouterCarteDefausse(Joueur *joueur, Carte carte) {
    if (joueur->nb_defausse >= joueur->capacite_defausse) {
        joueur->capacite_defausse *= 2;
        Carte *nouveauTas = realloc(joueur->defausse, joueur->capacite_defausse * sizeof(Carte));
        if (nouveauTas == NULL) {
            fprintf(stderr, "Erreur lors de l'agrandissement de la défausse du joueur.\n");
            exit(EXIT_FAILURE);
        }
        joueur->defausse = nouveauTas;
    }
    joueur->defausse[joueur->nb_defausse] = carte;
    joueur->nb_defausse++;
}
