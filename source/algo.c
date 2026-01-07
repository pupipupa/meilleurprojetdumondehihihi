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


TopK creer_topK(int k, InfoMem* infoMem){
    TopK topk;

    topk.classement = NULL;
    topk.occupes = 0;
    topk.k = k;   

    if(k<=0) return topk;

    Noeud** classement = (Noeud**)myMalloc(sizeof(Noeud*) * k, ...);
    if (!classement) {
        fprintf(stderr, "le malloc n'a pas marché\n");
        return topk;
    }
    topk.classement = classement;
    return topk;
}

int detruire_topK(TopK* topk, InfoMem * infoMem){
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

void parcours_arbre(Noeud * noeud, TopK * topk){
    if(!noeud) return;
    
    parcours_arbre(noeud->gauche, topk);
    mettre_a_jour_topK(noeud, topk);

    if(noeud->droite)
        parcours_arbre(noeud->droite, topk);
}

void print_topk(TopK * topk){
    if (!topk || topk->occupes == 0) return;

    int max_i = 0, i, kk; 
    int printed[topk->occupes] = {0};

    for(kk = 0; kk<topk->occupes; ++kk){
        max_i = 0;
        while (max_i < topk->occupes && printed[max_i] == 1) max_i++;
        if (max_i >= topk->occupes)
            break;

        i = 0;
        for(i = 1; i<topk->occupes; ++i){
            if(printed[i]!=1){
                if(topk->classement[i]->data.occurrences > topk->classement[max_i]->data.occurrences)
                max_i = i;
            }
        }
        printed[max_i] = 1;
        printf("%s: %d\n", topk->classement[max_i]->data.mot, topk->classement[max_i]->data.occurrences);
    }
}

static int est_lettre(char c){
    return (c >= 'a' && c <= 'z');
}

void inserer_texte_dans_arbre(char *texte, Arbre *arbre, InfoMem *mem){
    if(!texte || !arbre || !mem) return;

    char *p = texte;
    while(*p){
        while(*p && !est_lettre(*p)){
            p++;
        }
        if(!*p) return;
        char *start = p;
        while(*p && (est_lettre(*p) || *p == '-')){
            p++;
        }
        char tmp = *p;
        *p = '\0';
        insert_ou_incrementer(arbre, start, mem);
        *p = tmp;
    }
}