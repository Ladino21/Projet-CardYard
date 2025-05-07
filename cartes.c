#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "cartes.h"

Deck createDeckDefault() {
    Deck deck;
    // On définit un jeu de valeurs standard (0 à 13 avec 4 occurrences de chaque)
    int defaultValues[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    int defaultQuantities[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    int totalValues = 14;
    // Calculer le nombre total de cartes
    int totalCards = 0;
    for (int i = 0; i < totalValues; ++i) {
        totalCards += defaultQuantities[i];
    }
    if (totalCards > MAX_CARDS) {
        fprintf(stderr, "Trop de cartes dans le deck par défaut (total %d dépasse MAX_CARDS)\n", totalCards);
        totalCards = MAX_CARDS;
    }
    // Allouer le tableau de cartes
    deck.cards = malloc(totalCards * sizeof(Card));
    if (!deck.cards) {
        fprintf(stderr, "Échec d'allocation de la mémoire pour le deck\n");
        deck.size = 0;
        return deck;
    }
    deck.size = 0;
    // Remplir la pioche avec les valeurs spécifiées
    for (int i = 0; i < totalValues; ++i) {
        int valeur = defaultValues[i];
        int qty = defaultQuantities[i];
        for (int j = 0; j < qty && deck.size < totalCards; ++j) {
            deck.cards[deck.size].valeur = valeur;
            deck.cards[deck.size].visible = false;
            deck.size++;
        }
    }
    // Mélanger la pioche
    shuffleDeck(&deck);
    return deck;
}

Deck createDeckFromFile(const char *filename) {
    Deck deck;
    deck.cards = NULL;
    deck.size  = 0;

    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Impossible d'ouvrir '%s', deck par défaut.\n", filename);
        return createDeckDefault();
    }

    deck.cards = malloc(MAX_CARTES * sizeof(Card));
    if (!deck.cards) {
        printf("Échec d'allocation pour la pioche depuis fichier.\n");
        fclose(f);
        return createDeckDefault();
    }
    deck.size = 0;

    int value, quantity;
    while (fscanf(f, "%d:%d", &value, &quantity) == 2) {
        // Sentinelle : "0:0" termine la lecture
        if (value == 0 && quantity == 0) break;
        if (quantity < 0) quantity = 0;
        for (int i = 0; i < quantity && deck.size < MAX_CARTES; ++i) {
            deck.cards[deck.size].value     = value;
            deck.cards[deck.size].isVisible = false;
            deck.size++;
        }
    }

    fclose(f);
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
        // Échanger deck->cards[i] et deck->cards[j]
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

Card drawCard(Deck *deck) {
    Card card;
    card.valeur = -1;
    card.visible = false;
    if (!deck || deck->size == 0) {
        fprintf(stderr, "Tentative de piocher dans un deck vide.\n");
        return card;
    }
    // Renvoyer la carte du haut (dernier élément du tableau pour plus de simplicité)
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
