#include "cartes.h"
#include <stdlib.h>
#include <time.h>

// Initialise la pioche selon les règles du jeu (valeurs et quantités données)
void generer_pioche(Pioche *p) {
    p->nb_cartes = 0;
    // Ajouter les cartes de valeur -2
    for (int i = 0; i < NB_CARTES_NEG2; i++) {
        p->cartes[p->nb_cartes].valeur = -2;
        p->cartes[p->nb_cartes].visible = false;
        p->nb_cartes++;
    }
    // Ajouter les cartes de valeur -1
    for (int i = 0; i < NB_CARTES_NEG1; i++) {
        p->cartes[p->nb_cartes].valeur = -1;
        p->cartes[p->nb_cartes].visible = false;
        p->nb_cartes++;
    }
    // Ajouter les cartes de valeur 0
    for (int i = 0; i < NB_CARTES_ZERO; i++) {
        p->cartes[p->nb_cartes].valeur = 0;
        p->cartes[p->nb_cartes].visible = false;
        p->nb_cartes++;
    }
    // Ajouter les cartes de valeurs positives de 1 à VALEUR_MAX
    for (int v = 1; v <= VALEUR_MAX; v++) {
        for (int i = 0; i < NB_CARTES_POS; i++) {
            p->cartes[p->nb_cartes].valeur = v;
            p->cartes[p->nb_cartes].visible = false;
            p->nb_cartes++;
        }
    }
}

// Mélange la pioche en place avec l'algorithme de Fisher–Yates:contentReference[oaicite:2]{index=2}.
void melanger_pioche(Pioche *p) {
    // Initialiser la graine aléatoire pour varier le mélange à chaque exécution:contentReference[oaicite:3]{index=3}.
    srand((unsigned)time(NULL));
    for (int i = p->nb_cartes - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Échange des cartes aux indices i et j
        Carte temp = p->cartes[i];
        p->cartes[i] = p->cartes[j];
        p->cartes[j] = temp;
    }
}

// Pioche une carte (la retire de la fin du tableau) et la retourne.
// Si la pioche est vide, retourne une carte "nulle" (valeur 0, visible = false).
Carte piocher_carte(Pioche *p) {
    Carte c = {0, false};
    if (p->nb_cartes > 0) {
        p->nb_cartes--;
        c = p->cartes[p->nb_cartes];
    }
    return c;
}
