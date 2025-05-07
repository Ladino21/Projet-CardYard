#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "display.h"
#include "save.h"

// Initialise une nouvelle partie : crée la pioche et les joueurs, distribue les cartes initiales
void initialiserJeu(Jeu *jeu, int nbJoueurs) {
    jeu->nb_joueurs = nbJoueurs;
    // Créer la pioche et la mélanger
    jeu->pioche = creerPioche();
    jeu->nb_cartes_pioche = NB_CARTES_TOTAL;
    // Créer les joueurs
    jeu->joueurs = creerJoueurs(nbJoueurs);
    // Distribuer les cartes personnelles initiales à chaque joueur
    for (int j = 0; j < nbJoueurs; ++j) {
        for (int i = 0; i < NB_CARTES_PERSONNELLES; ++i) {
            // Tirer une carte de la pioche (prendre la dernière carte disponible)
            jeu->nb_cartes_pioche--;
            jeu->joueurs[j].cartes_personnelles[i] = jeu->pioche[jeu->nb_cartes_pioche];
            // Les cartes personnelles sont initialement toutes face cachée (cartes_visibles déjà à false)
        }
    }
}

// Lance la partie et gère la boucle de jeu. Renvoie 0 si la partie s'est terminée normalement, 1 si elle a été sauvegardée et interrompue.
int jouerPartie(Jeu *jeu) {
    int joueurActuel = 0;
    int partieTerminee = 0;
    while (!partieTerminee) {
        printf("\n----- Tour du joueur %d -----\n", joueurActuel + 1);
        afficherJeu(jeu, joueurActuel);
        // Demander l'action du joueur actuel
        printf("Joueur %d, choisissez une action (0 = piocher, numéro d'un joueur pour prendre sa défausse, 'S' pour sauvegarder et quitter) : ", joueurActuel + 1);
        char entree[10];
        if (!fgets(entree, sizeof(entree), stdin)) {
            continue; // en cas d'erreur de lecture
        }
        // Si le joueur choisit de sauvegarder
        if (entree[0] == 'S' || entree[0] == 's') {
            if (sauvegarderJeu("sauvegarde.txt", jeu) == 0) {
                printf("Partie sauvegardée dans \"sauvegarde.txt\". Fin du jeu.\n");
            } else {
                printf("Échec de la sauvegarde de la partie.\n");
            }
            return 1;
        }
        // Sinon, convertir l'entrée en nombre
        int choixSource = atoi(entree);
        Carte cartePiochee;
        if (choixSource == 0) {
            // Piocher depuis la pioche centrale
            if (jeu->nb_cartes_pioche > 0) {
                jeu->nb_cartes_pioche--;
                cartePiochee = jeu->pioche[jeu->nb_cartes_pioche];
                printf("Vous piochez une carte de la pioche centrale : ");
                afficherCarte(&cartePiochee, 1);
                printf("\n");
            } else {
                printf("La pioche est vide, choisissez une défausse.\n");
                continue;
            }
        } else if (choixSource >= 1 && choixSource <= jeu->nb_joueurs) {
            int jSource = choixSource - 1;
            if (jeu->joueurs[jSource].nb_defausse > 0) {
                // Prendre la carte du dessus de la défausse du joueur choisi
                cartePiochee = jeu->joueurs[jSource].defausse[jeu->joueurs[jSource].nb_defausse - 1];
                jeu->joueurs[jSource].nb_defausse--;
                printf("Vous prenez la carte %d de la défausse du joueur %d.\n", cartePiochee.valeur, jSource + 1);
            } else {
                printf("La défausse du joueur %d est vide.\n", jSource + 1);
                continue;
            }
        } else {
            printf("Choix invalide.\n");
            continue;
        }
        // Demander quelle carte personnelle échanger
        printf("Choisissez le numéro de la carte personnelle à échanger (1-%d) : ", NB_CARTES_PERSONNELLES);
        int position;
        if (scanf("%d", &position) != 1 || position < 1 || position > NB_CARTES_PERSONNELLES) {
            printf("Position invalide.\n");
            // Vider le reste de l'entrée si non valide
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }
        // Vider le reste de la ligne d'entrée
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        position--; // convertir en index 0
        // Échanger la carte
        Carte carteEchangee = jeu->joueurs[joueurActuel].cartes_personnelles[position];
        jeu->joueurs[joueurActuel].cartes_personnelles[position] = cartePiochee;
        // La nouvelle carte placée est maintenant face visible
        jeu->joueurs[joueurActuel].cartes_visibles[position] = true;
        // Ajouter la carte échangée à la défausse du joueur actuel
        ajouterCarteDefausse(&jeu->joueurs[joueurActuel], carteEchangee);
        printf("Vous défaussez la carte %d.\n", carteEchangee.valeur);
        // Vérifier si le joueur actuel a toutes ses cartes visibles
        int toutesVisibles = 1;
        for (int i = 0; i < NB_CARTES_PERSONNELLES; ++i) {
            if (!jeu->joueurs[joueurActuel].cartes_visibles[i]) {
                toutesVisibles = 0;
                break;
            }
        }
        if (toutesVisibles) {
            printf("\nLe joueur %d a retourné toutes ses cartes ! Fin de la partie.\n", joueurActuel + 1);
            // Rendre toutes les cartes visibles pour le calcul des scores
            for (int j = 0; j < jeu->nb_joueurs; ++j) {
                for (int i = 0; i < NB_CARTES_PERSONNELLES; ++i) {
                    jeu->joueurs[j].cartes_visibles[i] = true;
                }
            }
            // Calculer les scores finaux et déterminer le gagnant
            int meilleurScore = calculerScore(&jeu->joueurs[0]);
            int gagnant = 0;
            printf("\nScores finaux :\n");
            printf("Joueur 1 : %d\n", meilleurScore);
            for (int j = 1; j < jeu->nb_joueurs; ++j) {
                int scoreJ = calculerScore(&jeu->joueurs[j]);
                printf("Joueur %d : %d\n", j + 1, scoreJ);
                if (scoreJ < meilleurScore) {
                    meilleurScore = scoreJ;
                    gagnant = j;
                }
            }
            printf("Le gagnant est le joueur %d !\n", gagnant + 1);
            partieTerminee = 1;
        }
        // Passer au joueur suivant
        joueurActuel = (joueurActuel + 1) % jeu->nb_joueurs;
    }
    return 0;
}
