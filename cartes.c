//Adam
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "cartes.h"

// Crée une pioche avec des cartes par défautdans le cas ou l'utilisateur ne desire pas creer une pioche avec un fichier
Pioche creerPiocheDefaut() {
    Pioche pioche;

    int valeursDefaut[] = {-2,-1,0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int quantitesDefaut[] = {5, 10, 15, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10};
    int totalValeurs = 15;

    int totalCartes = 0;
    for (int i = 0; i < totalValeurs; ++i) {
        totalCartes += quantitesDefaut[i];
    }
    //vérification du nombre de cartes dans la pioche//
    if (totalCartes > NB_CARTES_MAX) {
        fprintf(stderr, "Trop de cartes dans la pioche par défaut (total %d dépasse NB_CARTES_MAX)\n", totalCartes);
        totalCartes = NB_CARTES_MAX;
    }
    //allocation dynamique du tableau possendant toutes les cartes de la pioche//
    pioche.cartes = malloc(totalCartes * sizeof(Carte));
    if (pioche.cartes==NULL) {
        //vérification
        fprintf(stderr, "Échec d'allocation de la mémoire pour la pioche\n");
        pioche.taille = 0;
        return pioche;
    }

    pioche.taille = 0;
    //remplissage automatique de la pioche par défaut.//
    for (int i = 0; i < totalValeurs; ++i) {
        int valeur = valeursDefaut[i];
        int quantite = quantitesDefaut[i];
        for (int j = 0; j < quantite && pioche.taille < totalCartes; ++j) {
            pioche.cartes[pioche.taille].valeur = valeur;
            pioche.cartes[pioche.taille].visible = false;
            pioche.taille++;
        }
    }

    melangerPioche(&pioche);
    return pioche;
}

// Crée une pioche à partir d’un fichier
Pioche creerPiocheDepuisFichier(const char *nomFichier) {
    //verif sur les pointeurs
    if (nomFichier == NULL) {
        printf("nomfichier invalide (NULL)\n");
        exit(1);
    }
    Pioche pioche;
    pioche.cartes = NULL;
    pioche.taille = 0;

    FILE *f = fopen(nomFichier, "r");
    //verif du fichier pour ne pas rentrer n'imoporte quoi
    if (f == NULL) {
        char rep = ' ';
        int buffer;
        //tant que l'utilisateur rentre n'importe quoi on ne continue pas 
        while (1) {
            printf("Impossible d'ouvrir \"%s\". Voulez-vous utiliser la pioche par défaut ? (o/n) : ", nomFichier);
            if (scanf(" %c", &rep) == 1) {
                rep = tolower(rep);
                //prend en compte le 1er caractere si l'utilisateur fait une faute de frappe et supprime dans la memoire le reste 
                while ((buffer = getchar()) != '\n' && buffer != EOF) {}

                if (rep == 'o') {
                    return creerPiocheDefaut();
                } else if (rep == 'n') {
                    printf("Annulation de la création de la partie.\n");
                    Pioche vide = { .cartes = NULL, .taille = 0 };
                    return vide;
                }
            }
            printf("Réponse invalide. Veuillez taper 'o' ou 'n'.\n");
        }
    }

    pioche.cartes = malloc(NB_CARTES_MAX * sizeof(Carte));
    if (pioche.cartes == NULL) {
        printf("Erreur d'allocation mémoire pour la pioche.\n");
        fclose(f);
        return creerPiocheDefaut();
    }

    int valeur = 0, quantite = 0;
    while (fscanf(f, "%d:%d", &valeur, &quantite) == 2) {
        if (valeur == 0 && quantite == 0) break; // condition d'arrêt
        if (quantite < 0) quantite = 0;

        for (int i = 0; i < quantite && pioche.taille < NB_CARTES_MAX; ++i) {
            pioche.cartes[pioche.taille].valeur = valeur;
            pioche.cartes[pioche.taille].visible = false;
            pioche.taille++;
        }
    }

    fclose(f);
    melangerPioche(&pioche);
    return pioche;
}

// Mélange les cartes de la pioche
void melangerPioche(Pioche *pioche) {
    //verif sur les pointeurs
    if (pioche == NULL) {
        printf("pioche invalide (NULL)\n");
        return;
    }
    if (pioche == NULL || pioche->taille <= 1) return;
    static int initialise = 0;
    if (!initialise) {
        srand((unsigned int) time(NULL));
        initialise = 1;
    }

    for (int i = pioche->taille - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        Carte temp = pioche->cartes[i];
        pioche->cartes[i] = pioche->cartes[j];
        pioche->cartes[j] = temp;
    }
}

// Tire une carte du dessus de la pioche
Carte piocherCarte(Pioche *pioche) {
    if (pioche == NULL) {
        printf("pioche invalide (NULL)\n");
        exit(2);
    }
    
    Carte carteInvalide;
    carteInvalide.valeur = -999;
    carteInvalide.visible = false;

    if (pioche == NULL || pioche->taille <= 0) {
        fprintf(stderr, "❌ Pioche vide !\n");
        return carteInvalide;
    }

    pioche->taille--;
    Carte carte = pioche->cartes[pioche->taille];
    carte.visible = false;

    return carte;
}


// Libère la mémoire allouée à la pioche
void libererPioche(Pioche *pioche) {
    if (pioche == NULL) {
        printf("pioche invalide (NULL)\n");
        exit(95);
    }
    if (pioche && pioche->cartes) {
        free(pioche->cartes);
        pioche->cartes = NULL;
    }
    if (pioche) {
        pioche->taille = 0;
    }
}
