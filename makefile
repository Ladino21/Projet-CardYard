# Nom de l'exécutable
EXEC = cardyard

# Liste des fichiers sources
SRCS = main.c cartes.c joueurs.c jeu.c affichage.c save.c

# Fichiers objets générés à partir des sources
OBJS = $(SRCS:.c=.o)

# Commande de compilation
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Règle principale
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	del /Q *.o *.exe 2>nul || true

# Règle pour forcer la recompilation complète
rebuild: clean all

.PHONY: all clean rebuild
