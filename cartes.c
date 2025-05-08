#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "cartes.h"

// Crée une pioche avec des cartes par défaut
Pioche creerPiocheDefaut() {
    Pioche pioche;

    int valeursDefaut[] = {-2,-1,0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,};
    int quantitesDefaut[] = {5, 10, 15, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,};
    int totalValeurs = 14;

    int totalCartes = 0;
    for (int i = 0; i < totalValeurs; ++i) {
        totalCartes += quantitesDefaut[i];
    }

    if (totalCartes > NB_CARTES_MAX) {
        fprintf(stderr, "Trop de cartes dans la pioche par défaut (total %d dépasse NB_CARTES_MAX)\n", totalCartes);
        totalCartes = NB_CARTES_MAX;
    }

    pioche.cartes = malloc(totalCartes * sizeof(Carte));
    if (!pioche.cartes) {
        fprintf(stderr, "Échec d'allocation de la mémoire pour la pioche\n");
        pioche.taille = 0;
        return pioche;
    }

    pioche.taille = 0;

    for (int i = 0; i < totalValeurs; ++i) {
        int valeur = valeursDefaut[i];
        int quantite = quantitesDefaut[i];
        for (int j = 0; j < quantite && pioche.taille < totalCartes; ++j) {
            pioche.cartes[pioche.taille].valeur = valeur;
            pioche.cartes[pioche.taille].visible = false;
            pioche.taille++;
        }
    }

    melangerPioche(&pioche);
    return pioche;
}

// Crée une pioche à partir d’un fichier
Pioche creerPiocheDepuisFichier(const char *nomFichier) {
    Pioche pioche;
    pioche.cartes = NULL;
    pioche.taille = 0;

    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        printf("Impossible d'ouvrir '%s', pioche par défaut utilisée.\n", nomFichier);
        return creerPiocheDefaut();
    }

    pioche.cartes = malloc(NB_CARTES_MAX * sizeof(Carte));
    if (!pioche.cartes) {
        printf("Échec d'allocation pour la pioche depuis le fichier.\n");
        fclose(fichier);
        return creerPiocheDefaut();
    }

    pioche.taille = 0;
    int valeur, quantite;

    while (fscanf(fichier, "%d:%d", &valeur, &quantite) == 2) {
        if (valeur == 0 && quantite == 0) break;
        if (quantite < 0) quantite = 0;
        for (int i = 0; i < quantite && pioche.taille < NB_CARTES_MAX; ++i) {
            pioche.cartes[pioche.taille].valeur = valeur;
            pioche.cartes[pioche.taille].visible = false;
            pioche.taille++;
        }
    }

    fclose(fichier);
    melangerPioche(&pioche);
    return pioche;
}

// Mélange les cartes de la pioche
void melangerPioche(Pioche *pioche) {
    if (!pioche || pioche->taille <= 1) return;
    static int initialise = 0;
    if (!initialise) {
        srand((unsigned int) time(NULL));
        initialise = 1;
    }

    for (int i = pioche->taille - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        Carte temp = pioche->cartes[i];
        pioche->cartes[i] = pioche->cartes[j];
        pioche->cartes[j] = temp;
    }
}

// Tire une carte du dessus de la pioche
Carte piocherCarte(Pioche *pioche) {
    Carte carte;
    carte.valeur = -1;
    carte.visible = false;

    if (!pioche || pioche->taille == 0) {
        fprintf(stderr, "Tentative de piocher dans une pioche vide.\n");
        return carte;
    }

    pioche->taille--;
    carte = pioche->cartes[pioche->taille];
    return carte;
}

// Libère la mémoire allouée à la pioche
void libererPioche(Pioche *pioche) {
    if (pioche && pioche->cartes) {
        free(pioche->cartes);
        pioche->cartes = NULL;
    }
    if (pioche) {
        pioche->taille = 0;
    }
}
