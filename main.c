#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "save.h"

int main() {
    printf(" Bienvenue dans CARD YARD !!\n");

    while (1) {
        printf("\nMenu principal :\n");
        printf("1. Nouvelle partie\n");
        printf("2. Charger une partie\n");
        printf("3. Quitter\n");
        printf("Votre choix : ");

        char ligne[64];
        int choix = 0;

        if (!fgets(ligne, sizeof(ligne), stdin)) {
            printf("Erreur de lecture.\n");
            continue;
        }

        if (sscanf(ligne, "%d", &choix) != 1) {
            printf("Entrée invalide. Veuillez entrer un nombre.\n");
            continue;
        }

        if (choix == 1) {
            int nb_joueurs = 0;
            int nb_cartes = 0;
            char reponse_fichier;
            char nom_fichier[256] = "";

            // Saisie du nombre de joueurs
            while (nb_joueurs < 2 || nb_joueurs > 8) {
                printf("Entrez le nombre de joueurs (2 à 8) : ");
                if (!fgets(ligne, sizeof(ligne), stdin)) continue;
                if (sscanf(ligne, "%d", &nb_joueurs) != 1) {
                    printf("Entrée invalide.\n");
                    nb_joueurs = 0;
                }
            }

            // Saisie du nombre de cartes par joueur
            while (nb_cartes < 1) {
                printf("Entrez le nombre de cartes personnelles par joueur : ");
                if (!fgets(ligne, sizeof(ligne), stdin)) continue;
                if (sscanf(ligne, "%d", &nb_cartes) != 1) {
                    printf("Entrée invalide.\n");
                    nb_cartes = 0;
                }
            }

            // Demande d’un fichier pour charger la pioche
            printf("Voulez-vous charger la pioche depuis un fichier ? (o/n) : ");
            if (!fgets(ligne, sizeof(ligne), stdin)) continue;
            reponse_fichier = ligne[0];

            if (reponse_fichier == 'o' || reponse_fichier == 'O') {
                printf("Nom du fichier de pioche : ");
                if (fgets(nom_fichier, sizeof(nom_fichier), stdin)) {
                    nom_fichier[strcspn(nom_fichier, "\n")] = '\0';
                }
            }

            Partie *partie = creerPartie(nb_joueurs, nb_cartes,
                (reponse_fichier == 'o' || reponse_fichier == 'O') ? nom_fichier : NULL);

            if (!partie) {
                printf("Échec de la création de la partie.\n");
                continue;
            }

            jouerPartie(partie);
            libererPartie(partie);

        } else if (choix == 2) {
            Partie *partie = chargerPartie("sauvegarde.dat");
            if (!partie) {
                printf("Aucune sauvegarde trouvée ou échec du chargement.\n");
            } else {
                printf("Partie chargée avec succès.\n");
                jouerPartie(partie);
                libererPartie(partie);
            }

        } else if (choix == 3) {
            printf("Au revoir !\n");
            break;
        } else {
            printf("Choix invalide. Réessayez.\n");
        }
    }

    return 0;
}
