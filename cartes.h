#ifndef CARTES_H
#define CARTES_H
typedef struct {
    int valeur;
    int visible; // 0 = caché, 1 = visible
} Carte;

// Initialise la pioche avec la répartition : 5x -2, 10x -1, 15x 0 et 10x (1..12)
void initialiser_pioche(Carte *pioche, int *nbCartes);

// Mélange le tableau de cartes (algorithme de Fisher-Yates)
void melanger(Carte *pioche, int nbCartes);

// Pioche la dernière carte du tableau
Carte piocher(Carte *pioche, int *nbCartes);
#endif
