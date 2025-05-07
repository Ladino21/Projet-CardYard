#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "cartes.h"

Deck createDeckDefault() {
    Deck deck;
    
    // On dÃ©finit un jeu de valeurs standard (-2 Ã  12 )
    
    int defaultValues[] = {-2,-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int defaultQuantities[] = {5,10,15,10,10,10,10,10,10,10,10,10,10,10,10};
    int totalValues = 15;
    // Calculer le nombre total de cartes
    int totalCards = 0;
    for (int i = 0; i < totalValues; ++i) {
        totalCards += defaultQuantities[i];
    }
    if (totalCards > MAX_CARDS) {
        fprintf(stderr, "Trop de cartes dans le deck par dÃ©faut (total %d dÃ©passe MAX_CARDS)\n", totalCards);
        totalCards = MAX_CARDS;
    }
    // Allouer le tableau de cartes
    deck.cards = malloc(totalCards * sizeof(Card));
    //verif
    if (!deck.cards) {
        fprintf(stderr, "Ã‰chec d'allocation de la mÃ©moire pour le deck\n");
        deck.size = 0;
        return deck;
    }
    deck.size = 0;
    // Remplir la pioche avec les valeurs spÃ©cifiÃ©es
    for (int i = 0; i < totalValues; ++i) {
        int value = defaultValues[i];
        int qty = defaultQuantities[i];
        for (int j = 0; j < qty && deck.size < totalCards; ++j) {
            //face visible
            deck.cards[deck.size].value = value; 
            //face caché
            deck.cards[deck.size].isVisible = false;
            deck.size++;
        }
    }
    // MÃ©langer la pioche
    shuffleDeck(&deck);
    return deck;
}

Deck createDeckFromFile(const char *filename) {
    Deck deck;
    deck.cards = NULL;
    deck.size = 0;
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s, utilisation de la pioche par dÃ©faut.\n", filename);
        // Si le fichier ne peut Ãªtre ouvert, on utilise le deck par dÃ©faut
        return createDeckDefault();
    }
    // D'abord, calculer le nombre total de cartes en lisant le fichier
    char line[128];
    int totalCards = 0;
    // Format attendu par ligne: "valeur:quantite"
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n' || line[0] == '\0') continue; // ignorer lignes vides
        char *colon = strchr(line, ':');
        if (!colon) continue; // ignorer lignes mal formatÃ©es
        *colon = '\0';
        int value = atoi(line);
        int quantity = atoi(colon + 1);
        if (quantity < 0) quantity = 0;
        totalCards += quantity;
    }
    if (totalCards > MAX_CARDS) {
        fprintf(stderr, "Trop de cartes dans le fichier (total %d dÃ©passe MAX_CARDS), troncature.\n", totalCards);
        totalCards = MAX_CARDS;
    }
    // Allouer le tableau de cartes
    deck.cards = malloc(totalCards * sizeof(Card));
    if (!deck.cards) {
        fprintf(stderr, "Ã‰chec d'allocation de la mÃ©moire pour le deck\n");
        deck.size = 0;
        fclose(f);
        return deck;
    }
    deck.size = 0;
    // Retour au dÃ©but du fichier pour lire et remplir le deck
    rewind(f);
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n' || line[0] == '\0') continue;
        char *colon = strchr(line, ':');
        if (!colon) continue;
        *colon = '\0';
        int value = atoi(line);
        int quantity = atoi(colon + 1);
        if (quantity < 0) quantity = 0;
        for (int j = 0; j < quantity && deck.size < totalCards; ++j) {
            deck.cards[deck.size].value = value;
            deck.cards[deck.size].isVisible = false;
            deck.size++;
        }
        if (deck.size >= totalCards) break;
    }
    fclose(f);
    // MÃ©langer le deck
    shuffleDeck(&deck);
    return deck;
}

void shuffleDeck(Deck *deck) {
    if (!deck || deck->size <= 1) return;
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int) time(NULL));
        seeded = 1;
    }
    for (int i = deck->size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        // Ã‰changer deck->cards[i] et deck->cards[j]
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

Card drawCard(Deck *deck) {
    Card card;
    card.value = -1;
    card.isVisible = false;
    if (!deck || deck->size == 0) {
        fprintf(stderr, "Tentative de piocher dans un deck vide.\n");
        return card;
    }
    // Renvoyer la carte du haut (dernier Ã©lÃ©ment du tableau pour plus de simplicitÃ©)
    deck->size--;
    card = deck->cards[deck->size];
    return card;
}

void freeDeck(Deck *deck) {
    if (deck && deck->cards) {
        free(deck->cards);
        deck->cards = NULL;
    }
    if (deck) {
        deck->size = 0;
    }
}
