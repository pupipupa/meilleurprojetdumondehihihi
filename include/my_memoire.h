//* les icnludes:

#ifndef MY_MEMOIRE_H
#define MY_MEMOIRE_H

#include <stdlib.h>

//* les structures:

typedef struct {
    size_t cumul_alloc;
    size_t cumul_desalloc;
    size_t max_alloc; //ils ont dit d'ajouter n'importe quel truc utile, mais jsp y'a quoi // Trust
} InfoMem;



//* les prototypes:

void* myMalloc(size_t size, InfoMem* infoMem);
void* myRealloc(void* ptr, size_t new_size, InfoMem* infoMem, size_t old_size);
void  myFree(void* ptr, InfoMem* infoMem, size_t old_size);

#endif