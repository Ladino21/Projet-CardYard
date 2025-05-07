#include <stdio.h>
#include <stdlib.h>
#include "save.h"

// Sauvegarde la partie actuelle dans le fichier spécifié. Renvoie 0 en cas de succès, 1 en cas d'erreur.
int sauvegarderJeu(const char *nomFichier, const Jeu *jeu) {
    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        return 1;
    }
    // Écrire le nombre de joueurs
    fprintf(f, "%d\n", jeu->nb_joueurs);
    // Écrire le nombre de cartes restantes dans la pioche, puis la liste des valeurs de ces cartes
    fprintf(f, "%d", jeu->nb_cartes_pioche);
    for (int i = 0; i < jeu->nb_cartes_pioche; ++i) {
        fprintf(f, " %d", jeu->pioche[i].valeur);
    }
    fprintf(f, "\n");
    // Pour chaque joueur, écrire ses cartes personnelles (valeur et visibilité) puis sa défausse
    for (int j = 0; j < jeu->nb_joueurs; ++j) {
        // Cartes personnelles (6 cartes : valeur et 0/1 pour visible)
        for (int i = 0; i < NB_CARTES_PERSONNELLES; ++i) {
            int vis = jeu->joueurs[j].cartes_visibles[i] ? 1 : 0;
            fprintf(f, "%d %d ", jeu->joueurs[j].cartes_personnelles[i].valeur, vis);
        }
        fprintf(f, "\n");
        // Défausse du joueur : nombre de cartes suivies de leurs valeurs
        fprintf(f, "%d", jeu->joueurs[j].nb_defausse);
        for (int k = 0; k < jeu->joueurs[j].nb_defausse; ++k) {
            fprintf(f, " %d", jeu->joueurs[j].defausse[k].valeur);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}

// Charge une partie depuis le fichier spécifié. Renvoie 0 en cas de succès, 1 en cas d'erreur.
int chargerJeu(const char *nomFichier, Jeu *jeu) {
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) {
        return 1;
    }
    // Lire le nombre de joueurs
    if (fscanf(f, "%d", &jeu->nb_joueurs) != 1) {
        fclose(f);
        return 1;
    }
    // Allouer les joueurs
    jeu->joueurs = creerJoueurs(jeu->nb_joueurs);
    // Lire le nombre de cartes restantes dans la pioche
    if (fscanf(f, "%d", &jeu->nb_cartes_pioche) != 1) {
        fclose(f);
        return 1;
    }
    // Allouer la pioche et lire les cartes restantes
    jeu->pioche = malloc(jeu->nb_cartes_pioche * sizeof(Carte));
    if (jeu->pioche == NULL) {
        fclose(f);
        return 1;
    }
    for (int i = 0; i < jeu->nb_cartes_pioche; ++i) {
        if (fscanf(f, "%d", &jeu->pioche[i].valeur) != 1) {
            fclose(f);
            return 1;
        }
    }
    // Pour chaque joueur, lire ses cartes personnelles et sa défausse
    for (int j = 0; j < jeu->nb_joueurs; ++j) {
        // Cartes personnelles
        for (int i = 0; i < NB_CARTES_PERSONNELLES; ++i) {
            int vis;
            if (fscanf(f, "%d %d", &jeu->joueurs[j].cartes_personnelles[i].valeur, &vis) != 2) {
                fclose(f);
                return 1;
            }
            jeu->joueurs[j].cartes_visibles[i] = (vis == 1);
        }
        // Défausse
        int nbDef;
        if (fscanf(f, "%d", &nbDef) != 1) {
            fclose(f);
            return 1;
        }
        // Ajuster la capacité de la défausse et lire les cartes de défausse
        free(jeu->joueurs[j].defausse);
        jeu->joueurs[j].capacite_defausse = (nbDef > 0 ? nbDef : 1);
        jeu->joueurs[j].defausse = malloc(jeu->joueurs[j].capacite_defausse * sizeof(Carte));
        if (jeu->joueurs[j].defausse == NULL) {
            fclose(f);
            return 1;
        }
        jeu->joueurs[j].nb_defausse = nbDef;
        for (int k = 0; k < nbDef; ++k) {
            if (fscanf(f, "%d", &jeu->joueurs[j].defausse[k].valeur) != 1) {
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);
    return 0;
}
