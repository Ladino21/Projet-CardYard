#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jeu.h"
#include "cartes.h"
#include "joueurs.h"
#include "save.h"
#include "affichage.c"


// Déclarations anticipées des fonctions d'affichage (implémentées dans display.c)
void afficherPartie(const Partie *partie);
void afficherJoueur(const Joueur *joueur, int indice);
void afficherCarte(const Carte *carte);

// Demande un entier entre min et max inclus
int demanderEntier(const char *message, int min, int max) {
    int valeur;
    char ligne[64];
    while (1) {
        printf("%s", message);
        if (!fgets(ligne, sizeof(ligne), stdin)) {
            return -1;
        }
        ligne[strcspn(ligne, "\n")] = '\0';
        if (sscanf(ligne, "%d", &valeur) == 1 && valeur >= min && valeur <= max) {
            return valeur;
        }
        printf("Veuillez entrer un nombre entre %d et %d.\n", min, max);
    }
}

// Crée une partie avec les paramètres spécifiés
Partie* creerPartie(int nbJoueurs, int nbCartesParJoueur, const char *fichierPioche) {
    Partie *partie = malloc(sizeof(Partie));
    if (!partie) {
        fprintf(stderr, "Erreur d'allocation pour la structure Partie\n");
        return NULL;
    }
    partie->nb_joueurs = nbJoueurs;
    partie->nb_cartes_personnelles = nbCartesParJoueur;
    partie->joueur_courant = 0;

    if (fichierPioche && strlen(fichierPioche) > 0) {
        partie->pioche = creerPiocheDepuisFichier(fichierPioche);
    } else {
        partie->pioche = creerPiocheDefaut();
    }

    if (nbJoueurs * nbCartesParJoueur > partie->pioche.taille) {
        printf("La pioche ne contient que %d cartes, insuffisant pour %d joueurs avec %d cartes chacun.\n",
               partie->pioche.taille, nbJoueurs, nbCartesParJoueur);
        free(partie);
        return NULL;
    }

    int totalCartes = partie->pioche.taille;
    partie->joueurs = creerJoueurs(nbJoueurs, nbCartesParJoueur, &partie->pioche, totalCartes);
    if (!partie->joueurs) {
        libererPioche(&partie->pioche);
        free(partie);
        return NULL;
    }
    return partie;
}

// Boucle principale du jeu
void jouerPartie(Partie *partie) {
    if (!partie) return;
    char entree[64];
    int finPartie = 0;

    while (!finPartie) {
        printf("\n===== Tour du joueur %d =====\n", partie->joueur_courant + 1);
        afficherPartie(partie);

        printf("Joueur %d, choisissez une action :\n", partie->joueur_courant + 1);
        printf("[0] Piocher dans la pioche centrale\n");
        printf("[1-%d] Piocher dans la défausse d’un joueur (entrez le numéro du joueur)\n", partie->nb_joueurs);
        printf("[S] Sauvegarder la partie\n");
        printf("[Q] Quitter\n");
        printf("Votre choix : ");

        if (!fgets(entree, sizeof(entree), stdin)) return;
        entree[strcspn(entree, "\n")] = '\0';

        if (strlen(entree) == 1) {
            char choixLettre = toupper(entree[0]);
            if (choixLettre == 'Q') {
                printf("Quitter la partie sans sauvegarder ? (o/n) : ");
                char confirmation;
                if (scanf(" %c", &confirmation) == 1 && tolower(confirmation) == 'o') {
                    printf("Partie terminée.\n");
                    return;
                }
                while (getchar() != '\n');
                continue;
            }
            if (choixLettre == 'S') {
                if (sauvegarderPartie("sauvegarde.dat", partie) == 0) {
                    printf("Partie sauvegardée dans sauvegarde.dat.\n");
                } else {
                    printf("Erreur lors de la sauvegarde.\n");
                }
                printf("Continuer la partie ? (o/n) : ");
                char reponse;
                if (scanf(" %c", &reponse) == 1 && tolower(reponse) != 'o') {
                    printf("Partie sauvegardée et terminée.\n");
                    return;
                }
                while (getchar() != '\n');
                continue;
            }
        }

        int choix = atoi(entree);
        Carte cartePiochee;
        int joueurSource = -1;

        if (choix == 0) {
            if (partie->pioche.taille == 0) {
                printf("La pioche est vide !\n");
                continue;
            }
            cartePiochee = piocherCarte(&partie->pioche);
            printf("Vous avez pioché la carte %d.\n", cartePiochee.valeur);
        } else if (choix >= 1 && choix <= partie->nb_joueurs) {
            int cible = choix - 1;
            if (partie->joueurs[cible].nb_defausse == 0) {
                printf("La défausse du joueur %d est vide.\n", cible + 1);
                continue;
            }
            printf("Cartes dans la défausse du joueur %d :\n", cible + 1);
            afficherLigneCartes(partie->joueurs[cible].defausse, partie->joueurs[cible].nb_defausse);

            int indexDefausse = demanderEntier("Choisissez l'index de la carte à prendre dans la défausse : ", 0, partie->joueurs[cible].nb_defausse - 1);

            // Récupérer la carte
            cartePiochee = partie->joueurs[cible].defausse[indexDefausse];

            // Décaler les cartes au-dessus
            for (int i = indexDefausse; i < partie->joueurs[cible].nb_defausse - 1; ++i) {
                partie->joueurs[cible].defausse[i] = partie->joueurs[cible].defausse[i + 1];
            }
            partie->joueurs[cible].nb_defausse--;
            joueurSource = cible;
            printf("Vous avez pris la carte %d de la défausse du joueur %d.\n", cartePiochee.valeur, cible + 1);
        } else {
            printf("Choix invalide.\n");
            continue;
        }

        cartePiochee.visible = true;
        int indexEchange = -1;

        if (choix == 0) {
            printf("Voulez-vous échanger cette carte avec une carte personnelle ? (o/n) : ");
            char rep;
            if (scanf(" %c", &rep) != 1 || tolower(rep) == 'n') {
                while (getchar() != '\n');
                indexEchange = -1;
            } else {
                indexEchange = demanderEntier("Index de la carte personnelle à échanger : ",
                                               0, partie->joueurs[partie->joueur_courant].nb_cartes - 1);
            }
        } else {
            indexEchange = demanderEntier("Index de la carte personnelle à échanger : ",
                                           0, partie->joueurs[partie->joueur_courant].nb_cartes - 1);
        }

        if (indexEchange >= 0) {
            if (partie->joueurs[partie->joueur_courant].personnelles[indexEchange].visible) {
                printf("Impossible d’échanger une carte déjà visible. Tour annulé.\n");
                if (joueurSource != -1) {
                    partie->joueurs[joueurSource].defausse[partie->joueurs[joueurSource].nb_defausse++] = cartePiochee;
                } else {
                    partie->pioche.cartes[partie->pioche.taille++] = cartePiochee;
                }
            } else {
                Carte carteRemplacee = partie->joueurs[partie->joueur_courant].personnelles[indexEchange];
                partie->joueurs[partie->joueur_courant].personnelles[indexEchange] = cartePiochee;
                partie->joueurs[partie->joueur_courant].personnelles[indexEchange].visible = true;

                carteRemplacee.visible = true;
                partie->joueurs[partie->joueur_courant].defausse[partie->joueurs[partie->joueur_courant].nb_defausse++] = carteRemplacee;

                printf("Vous avez échangé %d contre %d.\n", cartePiochee.valeur, carteRemplacee.valeur);
            }
        } else {
            partie->joueurs[partie->joueur_courant].defausse[partie->joueurs[partie->joueur_courant].nb_defausse++] = cartePiochee;
            printf("Vous avez défaussé la carte %d.\n", cartePiochee.valeur);
        }

        for (int i = 0; i < partie->nb_joueurs; ++i) {
            int visibles = 0;
            for (int j = 0; j < partie->joueurs[i].nb_cartes; ++j) {
                if (partie->joueurs[i].personnelles[j].visible) visibles++;
            }
            //retourne toute les cartes une fois qu'un joueur retourne toutes ses cartes et désigne un gagnant 
            if (visibles == partie->joueurs[i].nb_cartes) {
                printf("Le joueur %d a retourné toutes ses cartes. Fin de la partie.\n", i + 1);
                for (int j = 0; j < partie->nb_joueurs; ++j) {
                    for (int k = 0; k < partie->joueurs[j].nb_cartes; ++k) {
                        partie->joueurs[j].personnelles[k].visible = true;
                    }
                }
                printf("\n--- Révélation des cartes ---\n");
                afficherPartie(partie);
                // Calcul des scores
                typedef struct {
                    int indice;
                    int score;
                } Classement;
                Classement classement[NB_JOUEURS_MAX];

                for (int j = 0; j < partie->nb_joueurs; ++j) {
                    int score = 0;
                    for (int k = 0; k < partie->joueurs[j].nb_cartes; ++k) {
                        score += partie->joueurs[j].personnelles[k].valeur;
                    }
                    classement[j].indice = j;
                    classement[j].score = score;
                }
                 // Tri par score croissant (plus petit score = meilleur)
                for (int a = 0; a < partie->nb_joueurs - 1; ++a) {
                    for (int b = a + 1; b < partie->nb_joueurs; ++b) {
                        if (classement[a].score > classement[b].score) {
                            Classement temp = classement[a];
                            classement[a] = classement[b];
                            classement[b] = temp;
                        }
                    }
                }    
                // Affichage du classement
                printf("\n🏆 Classement final :\n");
                for (int j = 0; j < partie->nb_joueurs; ++j) {
                    printf("%d. Joueur %d avec %d points\n", j + 1, classement[j].indice + 1, classement[j].score);
                }
                printf("\n🎉 Le joueur %d remporte la partie ! 🎉\n", classement[0].indice + 1);
                finPartie = 1;
                break;
            }
        }

        partie->joueur_courant = (partie->joueur_courant + 1) % partie->nb_joueurs;
    }

    printf("État final :\n");
    afficherPartie(partie);
    printf("Merci d’avoir joué !\n");
}

// Libère toute la mémoire utilisée par la partie
void libererPartie(Partie *partie) {
    if (!partie) return;
    if (partie->joueurs) {
        libererJoueurs(partie->joueurs, partie->nb_joueurs);
        partie->joueurs = NULL;
    }
    libererPioche(&partie->pioche);
    free(partie);
}
