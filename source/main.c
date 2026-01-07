//*les included:

#include "projet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//* le main:

// MAIN FINAL ?!?!?! ( spoiler : non ) (OUIIIIIIIIIIII)

int main(int argc, char **argv) {

    //  Valeurs par défaut 

    size_t topN = 20;
    int algo = 1; // 1 = algo1, 2 = algo2, ? = algo3
    char *outfile = NULL;
    char *perffile = NULL;

    InfoMem mem_files = (InfoMem){0, 0, 0};
    size_t files_bytes = sizeof(char*) * (size_t)argc;
    char **files = (char **)myMalloc(files_bytes, &mem_files); // Tableau temporaire pour stocker les fichiers
    if (!files) {
        fprintf(stderr, "Erreur allocation tableau fichiers\n");
        return 1;
    }
    int nfiles = 0;

    // On lit les arguments de l'execution
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            myFree(files, &mem_files, files_bytes);
            return 0;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            topN = (size_t)atoi(argv[++i]);
            if (topN == 0) topN = 20;
        } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            ++i;
            if (strcmp(argv[i], "algo1") == 0) algo = 1;
            else if (strcmp(argv[i], "algo2") == 0) algo = 2;
            else if (strcmp(argv[i], "algo3") == 0) algo = 3;
            else {
                fprintf(stderr, "Algorithme inconnu '%s'\n", argv[i]);
                myFree(files, &mem_files, files_bytes);
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
        myFree(files, &mem_files, files_bytes);
        return 1;
    }

    // 
    for (int fi = 0; fi < nfiles; ++fi) {
        const char *input = files[fi];

        InfoMem mem = {0,0,0};

        size_t texte_len = 0;
        char *texte = ouvrir_file(input, &mem, &texte_len);

        if (!texte) {
            fprintf(stderr, "Impossible de lire '%s'\n", input);
            continue;
        }

        normalisation_texte_v2(texte);

        //!ALGO 3 ARBRES:
        if (algo == 3) {
            size_t total_mots = compter_mots(texte);

            Arbre arbre;
            arbre.racine = NULL;
            arbre.nb_mots_uniques = 0;

            clock_t t0 = clock();
            inserer_texte_dans_arbre(texte, &arbre, &mem);
            TopK topk = creer_topK((int)topN, &mem);
            parcours_arbre(arbre.racine, &topk);
            clock_t t1 = clock();
            double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;

            if (outfile) {
                FILE *out = fopen(outfile, "a");
                if (out) {
                    fprint_topk(out, &topk);
                    fprintf(out, "\n");
                    fclose(out);
                } else {
                    perror("fopen sortie");
                }
            } else {
                print_topk(&topk);
            }

            size_t total_alloc = mem.cumul_alloc + mem_files.cumul_alloc;
            size_t total_desalloc = mem.cumul_desalloc + mem_files.cumul_desalloc;
            size_t total_max = mem.max_alloc + mem_files.max_alloc;


            if (perffile) {
                ecrire_perf_csv(perffile,
                                "algo3",
                                input,
                                total_mots,
                                arbre.nb_mots_uniques,
                                elapsed,
                                total_alloc,
                                total_desalloc,
                                total_max);
            }

            detruire_topK(&topk, &mem);
            liberer_arbre(arbre.racine, &mem);
            myFree(texte, &mem, texte_len);
            printf("Fichier: %s | algo: algo3 | total_mots : %zu | distinct : %zu | time : %.6fs | max_mem : %zu bytes\n",
                input,
                total_mots,
                arbre.nb_mots_uniques,
                elapsed,
                total_max);
            continue;
        }


        Dico *dico = initDico(16, &mem);
        if (!dico) {
            fprintf(stderr, "Erreur initDico pour %s\n", input);
            myFree(texte, &mem, texte_len);
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

        if (algo == 1) {
            trierDicoParOccurences(dico); 
        }
        
        // Affichage ou fichier
        if (afficher_resultats(dico, topN, outfile, input) != 0) {
            fprintf(stderr, "Erreur écriture résultats pour %s\n", input);
        }

        size_t total_alloc = mem.cumul_alloc + mem_files.cumul_alloc;
        size_t total_desalloc = mem.cumul_desalloc + mem_files.cumul_desalloc;
        size_t total_max = mem.max_alloc + mem_files.max_alloc;

        
        if (perffile) { // Perf
            ecrire_perf_csv(perffile,
                            (algo == 1) ? "algo1" :
                            (algo == 2) ? "algo2" : "algo3", // A modifier pour l'ajout d'algo
                            input,
                            total_mots,
                            dico->nb_mots,
                            elapsed,
                            total_alloc,
                            total_desalloc,
                            total_max);
        }

        // Récapitulatif
        printf("Fichier : %s | algo: %s | total_mots : %zu | distinct : %zu | time : %.6fs | max_mem : %zu bytes\n",
               input,
               (algo == 1) ? "algo1" : "algo2", // A modifier pour l'ajout d'algo
               total_mots,
               dico->nb_mots,
               elapsed,
               total_max);

        freeDico(dico, &mem);
        myFree(texte, &mem, texte_len);
    }

    myFree(files, &mem_files, files_bytes);
    return 0;
}

