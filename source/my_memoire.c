//* les includes:

#include "my_memoire.h"
#include <stdlib.h>
#include <stdio.h>

//*les fonctions:

//!1:
void* myMalloc(size_t size, InfoMem* infoMem) //BUT : alloue une taille et met à jour la structure pour pouvoir retrouvé la mémoire utilisé
{
    //Pour quand même alloué un truc : evite malloc(0) -> provoque de potentiels bugs
    if (size == 0) {
        size = 1;
    }
    void* p = malloc(size);
    if (p != NULL && infoMem != NULL) { //regarde si allocation réussi et que structure existe
        infoMem->cumul_alloc += size; //Met à jour le compteur des allocations faites

        // Mémoire actuellement utilisée = alloc - désalloc
        size_t mem_actuelle =  infoMem->cumul_alloc - infoMem->cumul_desalloc;

        // Mise à jour du pic mémoire si nécessaire
        if (mem_actuelle > infoMem->max_alloc) infoMem->max_alloc = mem_actuelle;
        
    }
    return p; //NULL en cas d'échec
}

//!2:
void* myRealloc(void* ptr, size_t new_size, InfoMem* infoMem, size_t old_size) // BUT : recadre une taille et met à jour la structure en fonction des modifications d’agrandissement ou de rapetissement
{
    if (new_size == 0) { // Pour quand même allouer un truc : évite realloc(ptr, 0) -> provoque de potentiels bugs
        new_size = 1;
    }
    if (ptr == NULL) return myMalloc(new_size, infoMem); // realloc(NULL, size) ≡ malloc(size), donc on a juste à appeler myMalloc à la place
    void* p = realloc(ptr, new_size); // on fait realloc
    if (p != NULL && infoMem != NULL) { // On ne sait pas si realloc a agrandi ou rapetissé la taille, alors on va incrémenter les deux valeurs
        infoMem->cumul_alloc    += new_size;
        infoMem->cumul_desalloc += old_size;

        // Mémoire actuellement utilisée
        size_t mem_actuelle = infoMem->cumul_alloc - infoMem->cumul_desalloc;

        // idem myMalloc
        if (mem_actuelle > infoMem->max_alloc)
            infoMem->max_alloc = mem_actuelle;
    
    }
    return p; // NULL en cas d’échec
}


//!3:
void myFree(void* ptr, InfoMem* infoMem, size_t old_size) // BUT : libère un bloc mémoire et met à jour la structure
{
    if (ptr == NULL) return; // rien à faire si le pointeur est NULL

    if (infoMem != NULL) {
        infoMem->cumul_desalloc += old_size; // incrémente le compteur de mémoire désallouée
    } else {
        fprintf(stderr, "infoMem est NULL, cumul_desalloc non mis à jour\n"); // Message préventif
    }
    free(ptr); // libération de la mémoire
}


