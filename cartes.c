// cartes.c
#include <stdlib.h>
#include <time.h>
#include "cartes.h"

void initialiser_pioche(Carte *pioche, int *nbCartes) {
    int index = 0;
    // 5 cartes de -2
    for (int i = 0; i < 5; i++) {
        pioche[index].valeur = -2;
        pioche[index].visible = 0;
        index++;
    }
    // 10 cartes de -1
    for (int i = 0; i < 10; i++) {
        pioche[index].valeur = -1;
        pioche[index].visible = 0;
        index++;
    }
    // 15 cartes de 0
    for (int i = 0; i < 15; i++) {
        pioche[index].valeur = 0;
        pioche[index].visible = 0;
        index++;
    }
    // 10 cartes de chaque valeur positive (1 à 12)
    for (int val = 1; val <= 12; val++) {
        for (int i = 0; i < 10; i++) {
            pioche[index].valeur = val;
            pioche[index].visible = 0;
            index++;
        }
    }
    *nbCartes = index; // total = 150
}

void melanger(Carte *pioche, int nbCartes) {
    srand(time(NULL));
    for (int i = nbCartes - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Carte temp = pioche[i];
        pioche[i] = pioche[j];
        pioche[j] = temp;
    }
}

Carte piocher(Carte *pioche, int *nbCartes) {
    if (*nbCartes <= 0) {
        // Retourne une carte invalide si la pioche est vide
        Carte invalide;
        invalide.valeur = -999;
        invalide.visible = 0;
        return invalide;
    }
    (*nbCartes)--;
    return pioche[*nbCartes];
}
