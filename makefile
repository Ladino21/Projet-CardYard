CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o cartes.o joueurs.o jeu.o affichage.o save.o
EXEC = cardyard

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC)

run: all
	./$(EXEC)
