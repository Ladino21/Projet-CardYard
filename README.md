# Projet-CardYard

Etapes de compilation pour lancer ce jeu incroyable créer par Adam Abd El Ghany et nidal mekeddem :
Etape 1: Ouvrir un terminal
- Appuie sur Alt + F2
- Tape gnome-terminal puis appuie sur Entrée
Etape 2: Cloner le projet depuis GitHub
- Entrez "git clone https://github.com/Ladino21/Projet-CardYard"
Etape 3: Accéder au dossier du projet
- Entrez "gcc -o jeu *.c"
--> un executable est crée appelé jeu
Etape 4: Lancer le jeu
- Entrez "./jeu"

!! Attention !! Si vous souhaiter modifier le fichier "pioche.txt" afin de modifier le nombre de cartes pour chaque valeur pensez bien à laisser la derniere ligne "0:0" 

Règles du jeu – Card Yard

Card Yard est un jeu de cartes à 2 à 8 joueurs où chacun cherche à obtenir le score le plus faible possible à la fin de la partie.
But du jeu

Avoir la somme de points la plus basse avec ses cartes personnelles visibles à la fin de la partie.
Mise en place

    Chaque joueur reçoit un certain nombre de cartes personnelles (face cachée).

    Une pioche est créée (par défaut ou depuis un fichier).

    Chaque joueur a aussi une défausse, vide au départ.

Déroulement d’un tour

À chaque tour, un joueur peut :

    Piocher une carte (dans la pioche ou la défausse)

    Choisir de :

        Remplacer une de ses cartes personnelles (visible ou non)

        ou

        Défausser la carte sans rien changer

    Une fois une carte personnelle remplacée, elle devient visible.

Fin de partie

Quand toutes les cartes personnelles de tous les joueurs sont visibles, le jeu se termine.
Calcul du score

Chaque joueur additionne la valeur de ses cartes visibles.
Le joueur avec le score le plus bas gagne.

les valeurs et quantités de chaque
 carte sont fixées dans un fichier (de fait on peut modifier le
 jeu pour avoir un carte +20 au lieu du +12 par exemple) 
 vous pourrez retrouvez cette fonctionalié dans pioche.txt ne supprimez pas la ligne 0:0
