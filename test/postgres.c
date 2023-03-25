#include "lib/postgres.h"
#include <stdlib.h>

void *AllocSetAlloc(MemoryContext context, int size){
    void *a=malloc(size);
    return a;
}

void AllocSetFree(void *pointer){
    free(pointer);
    return ;
}

void *palloc(int size) {
    void *a=malloc(size);
    return a;
}

void pfree(void *pointer) {
    free(pointer);
    return ;
}