//*les includes:

#include "projet.h"
#include <string.h>
#include <stdio.h>

//*les fonctions:

//? les algos: 

//!1:

/*
int main(void)
{
    InfoMem mem = {0, 0};
    char *texte = ouvrir_file("C:\\goat\\meilleurprojetdumondehihihi\\data\\test.txt"); // Très très mid comme façon de faire mais même les génies ont leurs limites
    if (texte == NULL) {
        fprintf(stderr, "Erreur lecture fichier\n");
        return 1;
    }
    normalisation_texte_v2(texte);
    Dico* dico = initDico(16, &mem);
    if (dico == NULL) {
        free(texte);
        return 1;
    }
    ajouterMotTexteADico(texte, dico, &mem);
    printf("\n --- DICTIONNAIRE ---\n");
    for (size_t i = 0; i < dico->nb_mots; ++i) {
        printf("%s : %zu\n",
               dico->mots[i].mot,
               dico->mots[i].occurrences);
    }
    freeDico(dico, &mem);
    free(texte);
    printf("\n --- MEMOIRE --- \n");
    printf("Alloue : %zu octets\n", mem.cumul_alloc);
    printf("Desalloue : %zu octets\n", mem.cumul_desalloc);
    
    if (mem.cumul_alloc == mem.cumul_desalloc)
        printf("Memoire OK \n");
    else
        printf("Fuite memoire \n");

    return 0;
}

*/

/* int main(void) {
    InfoMem mem = {0, 0};
    const char* fichier = "../data/test.txt";
    size_t k = 20; // nombre de mots à afficher

    // Lecture du fichier
    char* texte = ouvrir_file(fichier);
    if (!texte) {
        fprintf(stderr, "Erreur lecture fichier\n");
        return 1;
    }

    // Normalisation du texte
    normalisation_texte_v2(texte);

    // Initialisation du dictionnaire
    Dico* dico = initDico(16, &mem);
    if (!dico) {
        free(texte);
        return 1;
    }

    // Remplissage du dictionnaire
    ajouterMotTexteADico(texte, dico, &mem);

    // Tri des mots par nombre d'occurrences décroissant
    trierDicoParOccurences(dico);

    // Affichage des k mots les plus fréquents
    printf("--- TOP %zu MOTS ---\n", k);
    for (size_t i = 0; i < dico->nb_mots && i < k; ++i) {
        printf("%s : %zu\n", dico->mots[i].mot, dico->mots[i].occurrences);
    }

    // Libération mémoire
    freeDico(dico, &mem);
    free(texte);

    // Bilan mémoire
    printf("\n --- MEMOIRE --- \n");
    printf("Alloue : %zu octets\n", mem.cumul_alloc);
    printf("Desalloue : %zu octets\n", mem.cumul_desalloc);
    
    if (mem.cumul_alloc == mem.cumul_desalloc)
        printf("Memoire OK \n");
    else
        printf("Fuite memoire \n");

    return 0;
}*/

//!3

//!4(?)

//I WILL TRY 
//ARBRE binaire: 

//? structures:oooooooooooooooooooooooooooooo

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

//? oooooooooooooooooooooooooooooooooooooo

//* fonctions 

Noeud* creer_noeud(char * mot, InfoMem* infoMem){
    Noeud * nouv_noeud = (Noeud*)myMalloc(sizeof(Noeud), infoMem);

    if(nouv_noeud == NULL){
        fprintf(stedrr, "erreur: malloc n'a pas réussi à allouer de la mémoire!");
        return NULL;
    }

    char* copy_mot = (char*)myMalloc(sizeof(char*) * strlen(mot));
    nouv_noeud -> data.mot = mot;//ok
    nouv_noeud -> data.occurrences = 1;
    nouv_noeud -> gauche = NULL;
    nouv_noeud -> droite = NULL;
    return nouv_noeud;
}

