#include <stdlib.h>

typedef struct {
    size_t cumul_alloc;
    size_t cumul_desalloc;
} InfoMem;

//mémoire_estimee = cumul_alloc - cumul_desalloc

void* myMalloc(size_t size, InfoMem* infoMem) //BUt : alloue et met à jour la structure pour pouvoir retrouvé la mémoire utilisé
{
    //Pour quand même alloué un truc : evite malloc(0) -> provoque de potentiels bugs
    if (size == 0) {
        size = 1;
    }
    void* p = malloc(size);
    if (p != NULL && infoMem != NULL) { //regarde si allocation réussi et que structure existe
        infoMem->cumul_alloc += size; //Met à jour le compteur des allocations faites
    }
    return NULL; //NULL en cas d'échec
}


void* myRealloc(void* ptr, size_t new_size, InfoMem* infoMem, size_t old_size);
void  myFree(void* ptr, InfoMem* infoMem, size_t old_size);


