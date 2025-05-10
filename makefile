# Makefile pour Windows
CC = gcc
CFLAGS = -Wall
EXEC = skyjo.exe

SRC = main.c affichage.c jeu.c save.c cartes.c joueurs.c
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	del /Q *.o $(EXEC)

run: all
	$(EXEC)
