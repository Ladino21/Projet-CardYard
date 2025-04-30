// game.c
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "cartes.h"
#include "joueurs.h"
#include "display.h"

#define TOTAL_CARTES 150
#define MIN_CARTES_RESTANTES 75

// Exemple d'initialisation et distribution
void lancer_partie() {
    int nb_joueurs, nb_cartes_par_joueur;
    
    // Demander le nombre de joueurs et le nombre de cartes personnelles
    printf("Nombre de joueurs (2-8) : ");
    scanf("%d", &nb_joueurs);
    if (nb_joueurs < 2 || nb_joueurs > 8) {
        printf("Nombre de joueurs invalide !\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Nombre de cartes personnelles par joueur : ");
    scanf("%d", &nb_cartes_par_joueur);
    if (nb_joueurs * nb_cartes_par_joueur > TOTAL_CARTES - MIN_CARTES_RESTANTES) {
        printf("Erreur : trop de cartes distribuées, la pioche doit rester au moins avec %d cartes.\n", MIN_CARTES_RESTANTES);
        exit(EXIT_FAILURE);
    }
    
    // Initialisation des cartes
    Carte *pioche = malloc(TOTAL_CARTES * sizeof(Carte));
    if(!pioche) {
        printf("Erreur d'allocation mémoire.\n");
        exit(EXIT_FAILURE);
    }
    int nb_cartes;
    initialiser_pioche(pioche, &nb_cartes);
    melanger(pioche, nb_cartes);
    
    // Initialisation des joueurs
    Joueur *joueurs = malloc(nb_joueurs * sizeof(Joueur));
    if (!joueurs) {
        printf("Erreur d'allocation mémoire pour les joueurs.\n");
        exit(EXIT_FAILURE);
    }
    initialiser_joueurs(joueurs, nb_joueurs, nb_cartes_par_joueur);
    
    // Distribution des cartes personnelles
    for (int i = 0; i < nb_joueurs; i++) {
        for (int j = 0; j < nb_cartes_par_joueur; j++) {
            joueurs[i].cartes[j] = piocher(pioche, &nb_cartes);
        }
        joueurs[i].nb_cartes = nb_cartes_par_joueur;
    }
    
    // Boucle de jeu simplifiée
    int partie_terminee = 0;
    while (!partie_terminee) {
        for (int i = 0; i < nb_joueurs; i++) {
            printf("\n--- Tour de %s ---\n", joueurs[i].nom);
            afficher_joueur(&joueurs[i]);
            
            // Pour l'instant, on simule un échange simple :
            if (nb_cartes > 0) {
                Carte nouvelle = piocher(pioche, &nb_cartes);
                printf("%s a pioché : ", joueurs[i].nom);
                afficher_carte(nouvelle);
                printf("\n");
                // On échange par exemple la première carte (simulateur)
                Carte temp = joueurs[i].cartes[0];
                joueurs[i].cartes[0] = nouvelle;
                // La carte échangée devient visible et va dans la défausse
                temp.visible = 1;
                joueurs[i].defausse[joueurs[i].nb_defausse++] = temp;
            } else {
                printf("La pioche est vide !\n");
            }
            
            // Vérifier si le joueur a toutes ses cartes visibles
            int toutes_visibles = 1;
            for (int j = 0; j < joueurs[i].nb_cartes; j++) {
                if (!joueurs[i].cartes[j].visible) {
                    toutes_visibles = 0;
                    break;
                }
            }
            if (toutes_visibles) {
                printf("\n%s a révélé toutes ses cartes !\n", joueurs[i].nom);
                partie_terminee = 1;
                break;
            }
        }
    }
    
    // Fin de partie : révélation des cartes et calcul des scores
    printf("\n--- Fin de la partie ---\n");
    for (int i = 0; i < nb_joueurs; i++) {
        reveler_joueur(&joueurs[i]);
        int score = calculer_score(&joueurs[i]);
        printf("%s - Score : %d\n", joueurs[i].nom, score);
    }
    
    // Libération des ressources
    free(pioche);
    free(joueurs);
}
