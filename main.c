include 

int main() {
    int choice;

    do {
        display_main_menu();
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                start_new_game();
                break;
            case 2:
                load_game();  // à coder si tu fais les sauvegardes
                break;
            case 3:
                printf("À bientôt !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 3);

    return 0;
}
