//*les includes:

#ifndef PROJET_H
#define PROJET_H

#include <stddef.h>
#include <stdio.h>
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

//? pour algo4:

typedef struct Noeud Noeud;

struct Noeud {
  Mot data;
  Noeud* gauche;  
  Noeud* droite;
};

typedef struct {
   Noeud* racine;
   size_t nb_mots_uniques;
  } Arbre;

typedef struct{
    Noeud** classement; 
    int k;
    int occupes;
} TopK;

//*: les prototypes

//? dicos
Dico* initDico(size_t capacite_initiale, InfoMem* mem);
void ajouterMot(Dico* dico, const char* mot, InfoMem* mem);
void freeDico(Dico* dico, InfoMem* mem);
void ajouterMotTexteADico(char* texte, Dico* dico, InfoMem* mem);
//!il faut l'ajouter sinon y'a trop de pertes de mémoire je crois

//Algo 2 : tri

int comparerMots(const void* mot1, const void* mot2);
void trierDicoParOccurences(Dico* dico);

//? files
char* ouvrir_file(const char* path, InfoMem* mem, size_t* out_len);
int normalisation_texte_v2(char* s);
int switch_fr(unsigned char b2);

//? algos 
//aucun encore fait ;((((
//!4
Noeud* creer_noeud(const char * mot, InfoMem* infoMem);
Noeud* insert_ou_incrementer(Arbre*arbre, const char* mot, InfoMem* infoMem);

void parcours_arbre(Noeud* noeud, TopK* topk);
int detruire_topK(TopK* topk, InfoMem* infoMem);
TopK creer_topK(int k, InfoMem* infoMem);
int mettre_a_jour_topK(Noeud* n, TopK* topk);
void inserer_texte_dans_arbre(char *texte, Arbre *arbre, InfoMem *mem);
void liberer_arbre(Noeud *noeud, InfoMem *mem);

//? print 
void printtestcontenu(const char* s);
void print_topk(TopK* topk);
void fprint_topk(FILE *out, TopK *topk);


void print_help(char *prog);
int afficher_resultats(Dico *dico, size_t nb_max_mots, const char *fichier_sortie, const char *nom_fichier_entree);
int ecrire_perf_csv(const char *chemin_perf, const char *nom_algo, const char *nom_fichier, size_t nb_mots_total, size_t nb_mots_distincts, double temps_exec, size_t mem_allouee, size_t mem_desallouee, size_t pic_memoire);
size_t compter_mots(const char *texte);


#endif
