// cartes.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cartes.h"

Pioche creerPiocheDefaut() {
    Pioche pioche;
    pioche.taille = 0;
    pioche.cartes = malloc(MAX_CARTES * sizeof(Carte));
    if (!pioche.cartes) {
        fprintf(stderr, "Erreur d'allocation pour la pioche par défaut.\n");
        exit(EXIT_FAILURE);
    }

    int valeurs[] = {-2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int quantites[] = {5, 10, 15, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
    int total = sizeof(valeurs) / sizeof(valeurs[0]);

    for (int i = 0; i < total; ++i) {
        for (int j = 0; j < quantites[i] && pioche.taille < MAX_CARTES; ++j) {
            pioche.cartes[pioche.taille].valeur = valeurs[i];
            pioche.cartes[pioche.taille].visible = false;
            pioche.taille++;
        }
    }

    melangerPioche(&pioche);
    return pioche;
}

Pioche creerPiocheDepuisFichier(const char *nomFichier) {
    Pioche pioche;
    pioche.cartes = NULL;
    pioche.taille = 0;

    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s. Utilisation de la pioche par défaut.\n", nomFichier);
        return creerPiocheDefaut();
    }

    int valeur, quantite;
    int totalCartes = 0;

    // Premier passage : compter le total de cartes
    while (fscanf(fichier, "%d:%d", &valeur, &quantite) == 2) {
        if (valeur == 0 && quantite == 0) break;
        if (quantite < 0) quantite = 0;
        totalCartes += quantite;
    }

    if (totalCartes > MAX_CARTES) {
        fprintf(stderr, "Trop de cartes (max = %d). Troncature.\n", MAX_CARTES);
        totalCartes = MAX_CARTES;
    }

    pioche.cartes = malloc(totalCartes * sizeof(Carte));
    if (!pioche.cartes) {
        fprintf(stderr, "Échec d'allocation mémoire pour la pioche.\n");
        fclose(fichier);
        pioche.taille = 0;
        return pioche;
    }
    pioche.taille = 0;

    rewind(fichier);
    while (fscanf(fichier, "%d:%d", &valeur, &quantite) == 2) {
        if (valeur == 0 && quantite == 0) break;
        if (quantite < 0) quantite = 0;

        for (int j = 0; j < quantite && pioche.taille < totalCartes; ++j) {
            pioche.cartes[pioche.taille].valeur = valeur;
            pioche.cartes[pioche.taille].visible = false;
            pioche.taille++;
        }
        if (pioche.taille >= totalCartes) break;
    }

    fclose(fichier);
    melangerPioche(&pioche);
    return pioche;
}

void melangerPioche(Pioche *pioche) {
    if (!pioche || pioche->taille <= 1) return;
    srand((unsigned int) time(NULL));
    for (int i = pioche->taille - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        Carte temp = pioche->cartes[i];
        pioche->cartes[i] = pioche->cartes[j];
        pioche->cartes[j] = temp;
    }
}

Carte piocherCarte(Pioche *pioche) {
    Carte carte = {0, false};
    if (!pioche || pioche->taille == 0) {
        fprintf(stderr, "Pioche vide.\n");
        return carte;
    }
    pioche->taille--;
    return pioche->cartes[pioche->taille];
}

void libererPioche(Pioche *pioche) {
    if (pioche && pioche->cartes) {
        free(pioche->cartes);
        pioche->cartes = NULL;
        pioche->taille = 0;
    }
}
