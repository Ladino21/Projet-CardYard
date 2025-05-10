# Projet-CardYard

étapes de compilation pour lancer ce jeu incroyable créer par Adam Abd El Ghany et nidal mekeddem :
- ouvrir le terminale grace à la commande ...
- cloner l'adresse de notre lien github afin de créer un fichier cardyard
- dans le terminale il faut acceder au fichier grace a la commande "cd Projet Card Yard"
- ensuite compiler jeu grace a la commande "gcc-o" ou bien 
- Attention si voulez utilisez le fichier pioche.txt afin de modifier le nombre de cartes pour chaque valeur pensez bien a laisser la derniere ligne "0:0" 

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
