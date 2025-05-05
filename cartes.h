// === cartes.h ===
#ifndef CARTES_H
#define CARTES_H

#define MAX_CARTES 150

typedef struct {
    int valeur;
    int visible;
} Carte;

void initialiser_pioche(Carte *pioche, int *nb);
void melanger_pioche(Carte *pioche, int nb);
Carte piocher_carte(Carte *pioche, int *nb);
void afficher_carte(Carte c);

#endif
