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

        int choix = 0;
        if (scanf("%d", &choix) != 1) {
            break; // Erreur de lecture ou EOF
        }

        // Vider le tampon
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}

        if (choix == 1) {
            int nb_joueurs = 0;
            int nb_cartes = 0;
            char reponse_fichier;
            char nom_fichier[256] = "";

            // Saisie du nombre de joueurs
            while (nb_joueurs < 2 || nb_joueurs > 8) {
                printf("Entrez le nombre de joueurs (2 à 8) : ");
                if (scanf("%d", &nb_joueurs) != 1) {
                    nb_joueurs = 0;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    continue;
                }
                while ((ch = getchar()) != '\n' && ch != EOF) {}
                if (nb_joueurs < 2 || nb_joueurs > 8) {
                    printf("Nombre invalide. Réessayez.\n");
                }
            }

            // Saisie du nombre de cartes par joueur
            while (nb_cartes < 1) {
                printf("Entrez le nombre de cartes personnelles par joueur : ");
                if (scanf("%d", &nb_cartes) != 1) {
                    nb_cartes = 0;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    continue;
                }
                while ((ch = getchar()) != '\n' && ch != EOF) {}
                if (nb_cartes < 1) {
                    printf("Nombre invalide.\n");
                }
            }

            // Demande d'un fichier pour charger la pioche
            printf("Voulez-vous charger la pioche depuis un fichier ? (o/n) : ");
            if (scanf(" %c", &reponse_fichier) != 1) {
                reponse_fichier = 'n';
            }
            while ((ch = getchar()) != '\n' && ch != EOF) {}

            if (reponse_fichier == 'o' || reponse_fichier == 'O') {
                printf("Nom du fichier de pioche : ");
                if (fgets(nom_fichier, sizeof(nom_fichier), stdin)) {
                    nom_fichier[strcspn(nom_fichier, "\n")] = '\0'; // Enlever le \n
                }
            }

            // Création de la partie
            Partie *partie = creerPartie(nb_joueurs, nb_cartes,
                            (reponse_fichier == 'o' || reponse_fichier == 'O') ? nom_fichier : NULL);
            if (!partie) {
                printf("Échec de la création de la partie.\n");
                continue;
            }

            // Lancer la partie
            jouerPartie(partie);
            // Libérer la mémoire
            libererPartie(partie);

        } else if (choix == 2) {
            // Charger une partie sauvegardée
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
