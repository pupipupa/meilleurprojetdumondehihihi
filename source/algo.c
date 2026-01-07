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

//? oooooooooooooooooooooooooooooooooooooo

//* fonctions 

Noeud* creer_noeud(const char * mot, InfoMem* infoMem){

    if (!mot) return NULL; //только ли нулль?

    Noeud * nouv_noeud = (Noeud*)myMalloc(sizeof(Noeud), infoMem);
    if(!nouv_noeud){
        fprintf(stderr, "erreur: malloc n'a pas réussi à allouer de la mémoire!");
        return NULL;
    }

    char* copy_mot = (char*)myMalloc(sizeof(char) * (strlen(mot)+1), infoMem);
    if(!copy_mot){
        fprintf(stderr, "erreur: malloc num2 n'a pas réussi à allouer de la mémoire!");
        myFree(nouv_noeud, infoMem, sizeof(Noeud));
        return NULL;
    }

    strcpy(copy_mot, mot);

    nouv_noeud -> data.mot = copy_mot;//ok
    nouv_noeud -> data.occurrences = 1;
    nouv_noeud -> gauche = NULL;
    nouv_noeud -> droite = NULL;
    return nouv_noeud;
}

Noeud* insert_ou_incrementer(Arbre*arbre, const char* mot, InfoMem* infoMem){
    if(!arbre || !mot) return NULL;

    if(!arbre->racine){
        arbre->racine = creer_noeud(mot, infoMem);
        if(!arbre->racine) return NULL;
        arbre->nb_mots_uniques++;
        return arbre->racine;
    }
    Noeud* cur = arbre->racine;

    while(1){//не 0 иначе выкл
        int comp = strcmp(mot, cur->data.mot); //обьявление compa в обязаловку
        if (comp == 0){
            cur->data.occurrences++;
            return cur;
        }
        else if(comp < 0){ //genre si mot < arbre mot
            if (!cur->gauche){
                cur->gauche = creer_noeud(mot, infoMem);
                if(!cur->gauche) return NULL;
                arbre->nb_mots_uniques++;
                return cur->gauche;
            }
            else cur = cur->gauche;
        }
        else{
            if (!cur->droite){
                cur->droite = creer_noeud(mot, infoMem);
                if(!cur->droite) return NULL;
                arbre->nb_mots_uniques++;
                return cur->droite;
            }
            else cur = cur->droite;
        }
    }
    
}

TopK creer_topK(int k, ){
    TopK topk;

    topk.classement = NULL;
    topk.occupes = 0;
    topk.k = k;   

    if(k<=0) return topk;

    Noeud** classement = (Noeud**)myMalloc(sizeof(Noeud*) * k, ...);
    if(classement){
        topk.classement = classement;
        fprintf(stderr, "le malloc n'a pas marché");
    }
    return topk;
}

int detruire_topK(topK* topk, InfoMem * infoMem){
    if(topk == NULL || topk->classement == NULL) return 0;

    myFree(topk->classement, infoMem, sizeof(Noeud*) * topk->k);

    topk->classement = NULL;
    topk->occupes = 0;
    topk->k = 0;   
    return 0;
}

int mettre_a_jour_topK(Noeud* n, TopK* topk){
    if(!n || !topk) return 1;
    if(topk->occupes < topk->k){
        topk->classement[topk->occupes++] = n;
        return 0;
    }

    int i, min_i = 0;
    for(i=1; i<topk->k; ++i){
        if(topk->classement[i]->data.occurrences < topk->classement[min_i]->data.occurrences) min_i = i;
    }

    if(n->data.occurrences > topk->classement[min_i]->data.occurrences){
        topk->classement[min_i] = n;    
    }

    return 0;
}