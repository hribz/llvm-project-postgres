#include "mcxt.h"
#include "memdebug.h"

void *AllocSetAlloc(MemoryContext context, int size);

void AllocSetFree(void *pointer);

void *palloc(int size);

void pfree(void *pointer);