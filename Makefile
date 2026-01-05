#https://perso.univ-lyon1.fr/jean-claude.iehl/Public/educ/Makefile.html
#https://dev.to/ashcript/comprendre-le-makefile-exemple-avec-le-langage-c-47n9

#c'est horrible euhh

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = source/main.c source/projet.c source/algo.c source/my_memoire.c
OBJ = $(SRC:.c=.o)

TARGET = projet

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

#NOTE: COMMENT CA MARCHE????? JUSTEMENT POUR COMPILER MTN T'AS BESOIN DECRIRE JUSTE //make// dans le terminal, un seul mot hihi
#NOTE: ensuite, pour clean tout compilé: //make clean//
#NOTE: enfin pour lancer le programmooo: //./projet(notre file créé après le truc MAKE) data/test.txt(le fichiier à analyser) 10(top nb de mots par exemple) 1(nb algo par exemple)//
#NOTE: "par exemple" cara on a pas encore ajouté le bon main avec la possibilité a choisir le fichier directement sur le terminal + ni les fncts pour les algos etcccccccc