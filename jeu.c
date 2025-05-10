#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jeu.h"
#include "cartes.h"
#include "joueurs.h"
#include "save.h"
#include "affichage.h"

// Déclarations anticipées
void afficherPartie(const Partie *partie);
void afficherJoueur(const Joueur *joueur, int indice);
void afficherCarte(const Carte *carte);

int demanderEntier(const char *message, int min, int max) {
    int valeur;
    char ligne[64];
    while (1) {
        printf("%s", message);
        if (fgets(ligne, sizeof(ligne), stdin) == NULL) return -1;
        ligne[strcspn(ligne, "\n")] = '\0';
        if (sscanf(ligne, "%d", &valeur) == 1 && valeur >= min && valeur <= max) return valeur;
        printf("Veuillez entrer un nombre entre %d et %d.\n", min, max);
    }
}

Partie* creerPartie(int nbJoueurs, int nbCartesParJoueur, const char *fichierPioche) {
    Partie *partie = malloc(sizeof(Partie));
    if (!partie) return NULL;

    partie->nb_joueurs = nbJoueurs;
    partie->nb_cartes_personnelles = nbCartesParJoueur;
    partie->joueur_courant = 0;

    partie->pioche = (fichierPioche && strlen(fichierPioche) > 0)
        ? creerPiocheDepuisFichier(fichierPioche)
        : creerPiocheDefaut();

    if (nbJoueurs * nbCartesParJoueur > partie->=pioche.taille) {
        printf("Pioche insuffisante.\n");
        libererPioche(&partie->pioche);
        free(partie);
        return NULL;
    }

    partie->joueurs = creerJoueurs(nbJoueurs, nbCartesParJoueur, &partie->pioche, partie->pioche.taille);
    if (!partie->joueurs) {
        libererPioche(&partie->pioche);
        free(partie);
        return NULL;
    }

    return partie;
}

void jouerPartie(Partie *partie) {
    if (!partie) return;
    char entree[64];
    int finPartie = 0;

    while (!finPartie) {
        printf("\n===== Tour du joueur %d =====\n", partie->joueur_courant + 1);
        afficherPartie(partie);

        int piocheVide = (partie->pioche.taille == 0);
        printf("Joueur %d, choisissez une action :\n", partie->joueur_courant + 1);
        if (!piocheVide) printf("[0] Piocher dans la pioche centrale\n");
        printf("[1-%d] Piocher dans la défausse d’un joueur\n", partie->nb_joueurs);
        printf("[S] Sauvegarder\n[Q] Quitter\nVotre choix : ");

        int choix = -1;
        while (1) {
            if (!fgets(entree, sizeof(entree), stdin)) return;
            entree[strcspn(entree, "\n")] = '\0';

            if (strlen(entree) == 1) {
                char c = toupper(entree[0]);
                if (c == 'S' || c == 'Q') {
                    choix = -2;
                    break;
                } else if (isdigit(c)) {
                    choix = c - '0';
                    if ((piocheVide && choix == 0) || choix < 0 || choix > partie->nb_joueurs) {
                        printf("Veuillez saisir les caractères associés à une commande valide.\nVotre choix : ");
                        continue;
                    }
                    break;
                }
            }
            printf("Veuillez saisir les caractères associés à une commande valide.\nVotre choix : ");
        }

        if (strlen(entree) == 1) {
            char choixLettre = toupper(entree[0]);
            if (choixLettre == 'Q') {
                printf("Quitter sans sauvegarder ? (o/n) : ");
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
                    printf("Sauvegarde réussie.\n");
                } else {
                    printf("Échec de la sauvegarde.\n");
                }
                printf("Continuer ? (o/n) : ");
                char reponse;
                if (scanf(" %c", &reponse) == 1 && tolower(reponse) != 'o') {
                    printf("Partie sauvegardée et arrêtée.\n");
                    return;
                }
                while (getchar() != '\n');
                continue;
            }
        }

        Carte cartePiochee;
        int joueurSource = -1;

        if (choix == 0) {
            cartePiochee = piocherCarte(&partie->pioche);
            if (cartePiochee.valeur == -999) continue;
            printf("Carte piochée : %d\n", cartePiochee.valeur);
        } else if (choix >= 1 && choix <= partie->nb_joueurs) {
            int cible = choix - 1;
            if (partie->joueurs[cible].nb_defausse == 0) {
                printf("Défausse vide.\n");
                continue;
            }

            int nb = partie->joueurs[cible].nb_defausse;
            for (int i = 0; i < nb; i += 10) {
                int count = (nb - i > 10) ? 10 : (nb - i);
                afficherLigneCartes(&partie->joueurs[cible].defausse[i], count);
            }

            int indexDefausse = demanderEntier("Choisissez l'index de la carte que vous desirez: ", 0, nb - 1);
            cartePiochee = partie->joueurs[cible].defausse[indexDefausse];
            for (int i = indexDefausse; i < nb - 1; i++) {
                partie->joueurs[cible].defausse[i] = partie->joueurs[cible].defausse[i + 1];
            }
            partie->joueurs[cible].nb_defausse--;
            joueurSource = cible;
        }

        cartePiochee.visible = true;
        int indexEchange = -1;

        if (choix == 0) {
            char ligne[64], rep = ' ';
            while (1) {
                printf("Échanger avec une carte personnelle ? (o/n) : ");
                if (fgets(ligne, sizeof(ligne), stdin)) {
                    if (sscanf(ligne, " %c", &rep) == 1 && (tolower(rep) == 'o' || tolower(rep) == 'n')) break;
                }
                printf("Entrée invalide. Tapez 'o' ou 'n'.\n");
            }
            if (tolower(rep) == 'o') {
                indexEchange = demanderEntier("Index de la carte personnelle : ", 0, partie->joueurs[partie->joueur_courant].nb_cartes - 1);
            }
        } else {
            indexEchange = demanderEntier("Index de la carte personnelle : ", 0, partie->joueurs[partie->joueur_courant].nb_cartes - 1);
        }

        if (indexEchange >= 0) {
            while (partie->joueurs[partie->joueur_courant].personnelles[indexEchange].visible) {
                printf("Carte déjà visible !\n");
                indexEchange = demanderEntier("Choisissez une carte NON visible : ", 0, partie->joueurs[partie->joueur_courant].nb_cartes - 1);
            }

            Carte carteRemplacee = partie->joueurs[partie->joueur_courant].personnelles[indexEchange];
            partie->joueurs[partie->joueur_courant].personnelles[indexEchange] = cartePiochee;
            partie->joueurs[partie->joueur_courant].personnelles[indexEchange].visible = true;

            carteRemplacee.visible = true;
            partie->joueurs[partie->joueur_courant].defausse[partie->joueurs[partie->joueur_courant].nb_defausse++] = carteRemplacee;

            printf("Échange effectué : %d ↔ %d\n", cartePiochee.valeur, carteRemplacee.valeur);
        } else {
            partie->joueurs[partie->joueur_courant].defausse[partie->joueurs[partie->joueur_courant].nb_defausse++] = cartePiochee;
            printf("Carte défaussée : %d\n", cartePiochee.valeur);
        }

        for (int i = 0; i < partie->nb_joueurs; ++i) {
            int visibles = 0;
            for (int j = 0; j < partie->joueurs[i].nb_cartes; ++j) {
                if (partie->joueurs[i].personnelles[j].visible) visibles++;
            }
            if (visibles == partie->joueurs[i].nb_cartes) {
                printf("Le joueur %d a retourné toutes ses cartes. Fin de la partie.\n", i + 1);
                finPartie = 1;
                break;
            }
        }

        partie->joueur_courant = (partie->joueur_courant + 1) % partie->nb_joueurs;
    }

    for (int i = 0; i < partie->nb_joueurs; ++i) {
        for (int j = 0; j < partie->joueurs[i].nb_cartes; ++j) {
            partie->joueurs[i].personnelles[j].visible = true;
        }
    }

    printf("État final :\n");
    afficherPartie(partie);
    afficherClassement(partie);
    printf("Merci d’avoir joué !\n");
}

void libererPartie(Partie *partie) {
    if (!partie) return;
    libererJoueurs(partie->joueurs, partie->nb_joueurs);
    libererPioche(&partie->pioche);
    free(partie);
}

