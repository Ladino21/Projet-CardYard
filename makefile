CC = gcc
CFLAGS = -Wall -Wextra -std=c99


EXEC = cardyard.exe


SRC = main.c cartes.c joueurs.c jeu.c affichage.c save.c
OBJ = $(SRC:.c=.o)

all: $(EXEC)


$(EXEC): $(OBJ)
    $(CC) $(CFLAGS) -o $@ $^

clean:
    del /Q *.o $(EXEC)


run: all
    $(EXEC)
