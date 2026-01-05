//*les includes:

#include "projet.h"
#include <string.h>
#include <stdio.h>

//*les fonctions:

//? les algos: 

//!1:

//blabla

//!2

//!3

//!4(?)

//I WILL TRY 
//ARBRE binaire: 

typedef struct Noeud Noeud;

struct {
  Mot data;
  Noeud* gauche;  
  Noeud* droite;

} Noeud;

typedef struct {
   Noeud* racine;
   size_t nb_mots_uniques;
  } Arbre;

typedef struct{
    Noeud** classement; 
    int k;
    int occupes;
} TopK;