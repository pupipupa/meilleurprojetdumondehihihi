//*les included:

#include "projet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


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

// MAIN FINAL ?!?!?! ( spoiler : non )

int main(int argc, char **argv) {

    //  Valeurs par défaut 

    size_t topN = 20;
    int algo = 1; // 1 = algo1, 2 = algo2, ? = algo3
    char *outfile = NULL;
    char *perffile = NULL;

    char **files = (char **)malloc(sizeof(char*) * argc);// Tableau temporaire pour stocker les fichiers
    if (!files) {
        fprintf(stderr, "Erreur allocation tableau fichiers\n");
        return 1;
    }
    int nfiles = 0;

    // On lit les arguments de l'execution
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            free(files);
            return 0;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            topN = (size_t)atoi(argv[++i]);
            if (topN == 0) topN = 20;
        } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            ++i;
            if (strcmp(argv[i], "algo1") == 0) algo = 1;
            else if (strcmp(argv[i], "algo2") == 0) algo = 2; // A modifier pour l'ajout d'algo
            else {
                fprintf(stderr, "Algorithme inconnu '%s'\n", argv[i]);
                free(files);
                return 1;
            }
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            outfile = argv[++i];
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            perffile = argv[++i];
        } else {
            files[nfiles++] = argv[i]; // Argument normal -> fichier
        }
    }

    if (nfiles == 0) {
        fprintf(stderr, "Aucun fichier fourni. Utilisez --help.\n");
        free(files);
        return 1;
    }

    // 
    for (int fi = 0; fi < nfiles; ++fi) {
        const char *input = files[fi];

        InfoMem mem = {0,0,0};

        char *texte = ouvrir_file(input);
        if (!texte) {
            fprintf(stderr, "Impossible de lire '%s'\n", input);
            continue;
        }

        normalisation_texte_v2(texte);

        Dico *dico = initDico(16, &mem);
        if (!dico) {
            fprintf(stderr, "Erreur initDico pour %s\n", input);
            free(texte);
            continue;
        }

        size_t total_mots = compter_mots(texte);

        clock_t t0 = clock();
        ajouterMotTexteADico(texte, dico, &mem);

        if (algo == 2) { // Tri pour algo2
            trierDicoParOccurences(dico);
        }
        clock_t t1 = clock();
        double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;

        // Affichage ou fichier
        if (afficher_resultats(dico, topN, outfile, input) != 0) {
            fprintf(stderr, "Erreur écriture résultats pour %s\n", input);
        }

        
        if (perffile) { // Perf
            ecrire_perf_csv(perffile,
                            (algo == 1) ? "algo1" : "algo2", // A modifier pour l'ajout d'algo
                            input,
                            total_mots,
                            dico->nb_mots,
                            elapsed,
                            mem.cumul_alloc,
                            mem.cumul_desalloc,
                            mem.max_alloc);
        }

        // Récapitulatif
        printf("Fichier : %s | algo: %s | total_mots : %zu | distinct : %zu | time : %.6fs | max_mem : %zu bytes\n",
               input,
               (algo == 1) ? "algo1" : "algo2", // A modifier pour l'ajout d'algo
               total_mots,
               dico->nb_mots,
               elapsed,
               mem.max_alloc);

        freeDico(dico, &mem);
        free(texte);
    }

    free(files);
    return 0;
}
