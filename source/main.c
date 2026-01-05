//*les included:

#include "projet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//* le main:

/*int main(){
    printf("CA MARCHE??????????");
    return 0;
}*/



//NOTE: ANCIEN MAIN QU IL FAUT ADAPTER:

/*

int main(void){
        
    char *content = ouvrir_file("test.txt");
    if(content == NULL){
        fprintf(stderr, "Erreur: ouvrir_file a échoué\n");
        return 1;
    }

    normalisation_texte_v2(content);
    printtestcontenu(content);

    free(content);
    return 0;
}


Main algo 1 :


int main(void)
{
    InfoMem mem = {0, 0};
    const char* fichier = "../data/test.txt";
    char* texte = ouvrir_file(fichier);
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


int main(void) {
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
} */
