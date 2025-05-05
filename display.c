// === display.c ===
#include <stdio.h>
#include "cartes.h"

void afficher_carte_stylisee(Carte c) {
    const char *color;
    if (!c.visible) {
        printf("\033[0m┌─────┐\n│  ?? │\n└─────┘\033[0m");
        return;
    }

    if (c.valeur >= 10)      color = "\033[31m"; // rouge
    else if (c.valeur >= 5)  color = "\033[33m"; // jaune
    else if (c.valeur >= 0)  color = "\033[32m"; // vert
    else if (c.valeur == -1) color = "\033[34m"; // bleu
    else if (c.valeur == -2) color = "\033[35m"; // magenta
    else                    color = "\033[0m";  // par défaut

    printf("%s┌─────┐\n", color);
    printf("│ %3d │\n", c.valeur);
    printf("└─────┘\033[0m");
}
