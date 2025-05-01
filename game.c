// Dans game.c
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "cartes.h"
#include "joueur.h"

#define TOTAL_CARTES 150
#define MIN_CARTES_RESTANTES 75

void lancer_partie() {
    int nb_joueurs, nb_cartes_par_joueur;
    char utiliserFichier;

    printf("Nombre de joueurs (2-8) : ");
    scanf("%d", &nb_joueurs);
    // Vérifier nb_joueurs ici...

    printf("Nombre de cartes personnelles par joueur : ");
    scanf("%d", &nb_cartes_par_joueur);
    if (nb_joueurs * nb_cartes_par_joueur > TOTAL_CARTES - MIN_CARTES_RESTANTES) {
        printf("Erreur : trop de cartes distribuées, la pioche doit rester au moins avec %d cartes.\n", MIN_CARTES_RESTANTES);
        exit(EXIT_FAILURE);
    }

    printf("Utiliser un fichier de configuration pour les cartes ? (O/N) : ");
    scanf(" %c", &utiliserFichier);

    Carte *pioche = malloc(TOTAL_CARTES * sizeof(Carte));
    int nb_cartes;
    if (utiliserFichier == 'O' || utiliserFichier == 'o') {
        initialiser_pioche_valueFile(pioche, &nb_cartes, "config.txt");
    } else {
        initialiser_pioche(pioche, &nb_cartes);
    }
    melanger(pioche, nb_cartes);

    // Initialisation des joueurs
    Joueur *joueurs = malloc(nb_joueurs * sizeof(Joueur));
    initialiser_joueurs(joueurs, nb_joueurs, nb_cartes_par_joueur);

    // Distribution
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
