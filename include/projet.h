//*les includes:

#ifndef PROJET_H
#define PROJET_H

#include <stddef.h>
#include "my_memoire.h"


//*: les structures
typedef struct {
    char* mot; // le mot lui-même
    size_t occurrences; // combien de fois ce mot apparaît
} Mot;

typedef struct {
    Mot* mots; // tableau dynamique de mots -> structure de structure
    size_t nb_mots; // combien de mots sont stockés actuellement
    size_t taille; // capacité actuelle du tableau
} Dico;

//*: les prototypes

//? dicos
Dico* initDico(size_t capacite_initiale, InfoMem* mem);
void ajouterMot(Dico* dico, const char* mot, InfoMem* mem);
//une fonction libererDico(...); 
//!il faut l'ajouter sinon y'a trop de pertes de mémoire je crois

//? files
char* ouvrir_file(const char* path);
//char* ouvrir_file(const char* path, InfoMem* mem, size_t* out_len); //ce prototype pour une nouv version de la fnct avec les myMalloc/free pas encore modifiée
int normalisation_texte_v2(char* s);
int switch_fr(unsigned char b2);

//? algos 
//aucun encore fait ;((((

//? print 
void printtestcontenu(const char* s);

#endif
