#include <stdio.h>
#include "game.h"
#include "display.h"

int main() {
    int choix = 0;
    do {
        display_menu();  // affiche le menu principal
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                lancer_partie();
                break;
            case 2:
                // chargement de partie (optionnel)
                printf("Fonctionnalité de chargement non implémentée.\n");
                break;
            case 3:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Réessaie.\n");
        }
    } while (choix != 3);
    return 0;
}
