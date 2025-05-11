//Adam
#include <stdio.h>
#include <stdlib.h>
#include "joueurs.h"

Joueur* creerJoueurs(int nb_joueurs, int nb_cartes_par_joueur, Pioche *pioche, int nb_cartes_total) {
    //verif des pointeurs
    if (pioche == NULL) {
        printf("pioche invalide (NULL)\n");
        exit(9);
    }

    if (nb_joueurs < 2 || nb_joueurs > NB_JOUEURS_MAX) {
        printf("Nombre de joueurs invalide : %d. Doit être entre 2 et %d.\n", nb_joueurs, NB_JOUEURS_MAX);
        return NULL;
    }

    // Allouer le tableau de joueurs
    Joueur *joueurs = malloc(nb_joueurs * sizeof(Joueur));
    if (joueurs == NULL) {
        printf("Échec d'allocation pour les joueurs.\n");
        return NULL;
    }

    for (int i = 0; i < nb_joueurs; ++i) {
        joueurs[i].nb_cartes = nb_cartes_par_joueur;

        // Allouer les cartes personnelles
        joueurs[i].personnelles = malloc(nb_cartes_par_joueur * sizeof(Carte));
        if (joueurs[i].personnelles==NULL) {
            printf("Échec d'allocation pour les cartes personnelles du joueur %d\n", i);
            for (int k = 0; k < i; ++k) {
                free(joueurs[k].personnelles);
            }
            free(joueurs);
            return NULL;
        }

        // Distribuer les cartes personnelles depuis la pioche
        for (int c = 0; c < nb_cartes_par_joueur; ++c) {
            Carte carte = piocherCarte(pioche);
            carte.visible = false; // les cartes sont face cachée au début
            joueurs[i].personnelles[c] = carte;
        }

        // Allouer la défausse du joueur
        if (nb_cartes_total <= 0){
            nb_cartes_total = 1;
        }    
        joueurs[i].defausse = malloc(nb_cartes_total * sizeof(Carte));
        if (joueurs[i].defausse==NULL) {
            printf("Échec d'allocation pour la défausse du joueur %d\n", i);
            free(joueurs[i].personnelles);
            for (int k = 0; k < i; ++k) {
                free(joueurs[k].personnelles);
                free(joueurs[k].defausse);
            }
            free(joueurs);
            return NULL;
        }

        joueurs[i].nb_defausse = 0;
    }

    return joueurs;
}

void libererJoueurs(Joueur *joueurs, int nb_joueurs) {
    if (joueurs == NULL){
        //verif des pointeurs
        printf("joueurs invalide (NULL)\n");
        exit(10);
    }


    for (int i = 0; i < nb_joueurs; ++i) {
        if (joueurs[i].personnelles) {
            free(joueurs[i].personnelles);
            joueurs[i].personnelles = NULL;
        }
        if (joueurs[i].defausse) {
            free(joueurs[i].defausse);
            joueurs[i].defausse = NULL;
        }
    }

    free(joueurs);
}
