#include <stdio.h>
#include "display.h"

void display_menu() {
    printf("\n===== CARD YARD =====\n");
    printf("1. Nouvelle partie\n");
    printf("2. Charger une partie\n");
    printf("3. Quitter\n");
    printf("Choix : ");
}

void afficher_carte(Carte c) {
    if (c.visible){
        printf("[ %d ]", c.valeur);
    }
    else {
        printf("[ ? ]");
    }
}
