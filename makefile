CC = gcc
CFLAGS = -Wall -Wextra -std=c99

Nom de l'exécutable
EXEC = cardyard.exe

Fichiers sources
SRC = main.c cartes.c joueurs.c jeu.c affichage.c save.c
OBJ = $(SRC:.c=.o)

Règle par défaut
all: $(EXEC)

Lien des objets vers l'exécutable
$(EXEC): $(OBJ)
    $(CC) $(CFLAGS) -o $@ $^

Nettoyage des fichiers objets et exécutable
clean:
    del /Q *.o $(EXEC)

Compilation et exécution directe
run: all
    $(EXEC)
