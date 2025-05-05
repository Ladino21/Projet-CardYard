// === cartes.c ===
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cartes.h"

void initialiser_pioche(Carte *pioche, int *nb) {
    int index = 0;
    for (int i = 0; i < 5; i++) pioche[index++] = (Carte){-2, 0};
    for (int i = 0; i < 10; i++) pioche[index++] = (Carte){-1, 0};
    for (int i = 0; i < 15; i++) pioche[index++] = (Carte){0, 0};
    for (int val = 1; val <= 12; val++)
        for (int i = 0; i < 10; i++)
            pioche[index++] = (Carte){val, 0};
    *nb = index;
}

void melanger_pioche(Carte *pioche, int nb) {
    srand(time(NULL));
    for (int i = nb - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Carte temp = pioche[i];
        pioche[i] = pioche[j];
        pioche[j] = temp;
    }
}

Carte piocher_carte(Carte *pioche, int *nb) {
    if (*nb <= 0) return (Carte){-999, 0};
    return pioche[--(*nb)];
}

void afficher_carte(Carte c) {
    if (c.visible)
        printf("[ %2d ]", c.valeur);
    else
        printf("[ ?? ]");
}
