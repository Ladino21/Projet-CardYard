CC = gcc
CFLAGS = -std=c99 -g
EXEC = skyjo.exe

SRC = main.c affichage.c jeu.c save.c cartes.c joueurs.c
OBJ = $(SRC:.c=.o)


ifeq ($(OS),Windows_NT)
	RM = del /Q
	EXEC_RUN = $(EXEC)
else
	RM = rm -f
	EXEC_RUN = ./skyjo.exe
endif

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o $(EXEC)

run: all
	$(EXEC_RUN)
