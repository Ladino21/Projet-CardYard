# Nom du compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c99 -g

# Liste des fichiers source
SRC = main.c jeu.c cartes.c joueurs.c affichage.c save.c

# Liste des fichiers objets
OBJ = $(SRC:.c=.o)

# Nom de l'exécutable final (avec .exe pour Windows)
EXEC = cardyard.exe

# Cible par défaut
all: $(EXEC)

# Comment créer l'exécutable à partir des fichiers objets
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Comment compiler chaque fichier .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Supprime les fichiers objets et l'exécutable
clean:
	del /Q *.o *.exe 2>nul || exit 0

.PHONY: all clean
