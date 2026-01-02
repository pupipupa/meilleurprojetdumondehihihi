#include <stdlib.h>

typedef struct {
    size_t cumul_alloc;
    size_t cumul_desalloc;
} InfoMem;

//mémoire_estimee = cumul_alloc - cumul_desalloc

void* myMalloc(size_t size, InfoMem* infoMem) //BUT : alloue une taille et met à jour la structure pour pouvoir retrouvé la mémoire utilisé
{
    //Pour quand même alloué un truc : evite malloc(0) -> provoque de potentiels bugs
    if (size == 0) {
        size = 1;
    }
    void* p = malloc(size);
    if (p != NULL && infoMem != NULL) { //regarde si allocation réussi et que structure existe
        infoMem->cumul_alloc += size; //Met à jour le compteur des allocations faites
    }
    return p; //NULL en cas d'échec
}

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
    }
    return p; // NULL en cas d’échec
}


void  myFree(void* ptr, InfoMem* infoMem, size_t old_size);


