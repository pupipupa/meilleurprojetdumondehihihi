#include <stdlib.h>

typedef struct {
    size_t cumul_alloc;
    size_t cumul_desalloc;
    //
} InfoMem;

void* myMalloc(size_t size, InfoMem* infoMem);
void* myRealloc(void* ptr, size_t new_size, InfoMem* infoMem, size_t old_size);
void  myFree(void* ptr, InfoMem* infoMem, size_t old_size);


