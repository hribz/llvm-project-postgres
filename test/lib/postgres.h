#include "mcxt.h"
#include "memdebug.h"

void *AllocSetAlloc(MemoryContext context, int size);

void AllocSetFree(void *pointer);

void *palloc(void *pointer, int size);

void pfree(void *pointer, int size);