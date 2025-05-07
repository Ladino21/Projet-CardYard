#include <stdio.h>
#include <stdlib.h>
#include "cartes.h"

/**
 * Crée une nouvelle pioche avec la distribution de cartes par défaut.
 * Valeurs des cartes de -2 à 12 avec les quantités prédéfinies.
 */
Pioche* creerPiocheDefaut(void) {
    // Valeurs de carte par défaut et leurs quantités correspondantes
    int valeurs[] = { -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    int quantites[] = { 5, 10, 15, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
    int n = sizeof(valeurs) / sizeof(valeurs[0]);
    // Calcul du nombre total de cartes
    int nb_total = 0;
    for (int i = 0; i < n; ++i) {
        nb_total += quantites[i];
    }
    // Allocation de la pioche
    Pioche *pioche = malloc(sizeof(Pioche));
    if (pioche == NULL) {
        return NULL;
    }
    pioche->cartes = malloc(nb_total * sizeof(Carte));
    if (pioche->cartes == NULL) {
        free(pioche);
        return NULL;
    }
    pioche->nb_cartes = nb_total;
    // Remplissage de la pioche avec les cartes
    int index = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < quantites[i]; ++j) {
            pioche->cartes[index].valeur = valeurs[i];
            index++;
        }
    }
    return pioche;
}

/**
 * Crée une nouvelle pioche en lisant la distribution de cartes depuis un fichier.
 * Chaque ligne du fichier doit être au format "valeur:quantite" et la fin de la liste est marquée par "0:0".
 */
Pioche* creerPiocheDepuisFichier(const char *nomFichier) {
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) {
        return NULL;
    }
    // Allocation initiale de la pioche
    Pioche *pioche = malloc(sizeof(Pioche));
    if (pioche == NULL) {
        fclose(f);
        return NULL;
    }
    pioche->cartes = NULL;
    pioche->nb_cartes = 0;
    int valeur, quantite;
    // Lecture du fichier ligne par ligne sous la forme "valeur:quantite"
    while (fscanf(f, "%d:%d", &valeur, &quantite) == 2) {
        if (valeur == 0 && quantite == 0) {
            // Fin de la liste des cartes
            break;
        }
        // Réallocation de la mémoire pour ajouter les nouvelles cartes
        Carte *nouvellesCartes = realloc(pioche->cartes, (pioche->nb_cartes + quantite) * sizeof(Carte));
        if (nouvellesCartes == NULL) {
            // Libération de la mémoire en cas d'échec de réallocation
            free(pioche->cartes);
            free(pioche);
            fclose(f);
            return NULL;
        }
        pioche->cartes = nouvellesCartes;
        // Ajout des nouvelles cartes de la valeur donnée
        for (int i = 0; i < quantite; ++i) {
            pioche->cartes[pioche->nb_cartes + i].valeur = valeur;
        }
        pioche->nb_cartes += quantite;
    }
    fclose(f);
    return pioche;
}

/**
 * Mélange aléatoirement les cartes d'une pioche.
 */
void melangerPioche(Pioche *pioche) {
    if (pioche == NULL) {
        return;
    }
    // Mélange les cartes de la pioche de manière aléatoire
    for (int i = pioche->nb_cartes - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        Carte temp = pioche->cartes[i];
        pioche->cartes[i] = pioche->cartes[j];
        pioche->cartes[j] = temp;
    }
}

/**
 * Pioche une carte depuis la pioche (retire la dernière carte) et renvoie sa valeur.
 */
int piocherCarte(Pioche *pioche) {
    // Retire la dernière carte de la pioche et renvoie sa valeur
    pioche->nb_cartes--;
    return pioche->cartes[pioche->nb_cartes].valeur;
}

/**
 * Libère la mémoire allouée pour une pioche.
 */
void libererPioche(Pioche *pioche) {
    if (pioche != NULL) {
        free(pioche->cartes);
        free(pioche);
    }
}

