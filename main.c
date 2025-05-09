#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "save.h"
#include "cartes.h"

int main() {
    printf(" Bienvenue dans CARD YARD !!\n");

    while (1) {
        // ... (le menu principal, inchangé) ...

        if (choix == 1) {
            int nb_joueurs = 0;
            int nb_cartes = 0;
            char reponse_fichier;
            char nom_fichier[256] = "";

            // ... (saisie du nombre de joueurs et cartes inchangée) ...

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

            // [CORRECTION] Vérification de la distribution possible (erreur 2)
            {
                Pioche pioche_temp;
                if (reponse_fichier == 'o' || reponse_fichier == 'O') {
                    pioche_temp = creerPiocheDepuisFichier(nom_fichier);
                } else {
                    pioche_temp = creerPiocheDefaut();
                }
                while (nb_joueurs * nb_cartes > pioche_temp.taille) {
                    printf("La pioche ne contient que %d cartes, insuffisant pour %d joueurs avec %d cartes chacun.\n",
                           pioche_temp.taille, nb_joueurs, nb_cartes);
                    // Redemander le nombre de cartes personnelles
                    nb_cartes = 0;
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
                }
                libererPioche(&pioche_temp);
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
            // Charger une partie sauvegardée (inchangé)
            Partie *partie = chargerPartie("sauvegarde.dat");
            // ...
        } else if (choix == 3) {
            printf("Au revoir !\n");
            break;
        }
    }

    return 0;
}
