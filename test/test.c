#include "lib/postgres.h"
#include "/home/xiaoyu/gradDesign/llvm-project-postgres/build/lib/clang/13.0.1/include/sanitizer/asan_interface.h"
#include <stdio.h>

int main(){
    // MemoryContext m;
    // void *a=AllocSetAlloc(m,30);
    // VALGRIND_MAKE_MEM_NOACCESS(a,8);
    // VALGRIND_MAKE_MEM_DEFINED(a,1);
    // *((char *)a)='x';
    // AllocSetFree(a);

    // char my_char = 'a';
    // ASAN_POISON_MEMORY_REGION(&my_char, sizeof(char));
    // (my_char)='x';

    char *a=(char *)malloc(32);
    // char *b=(char *)palloc(32);
    printf("0x%llx\n",(void *)a);
    void *b=((void *) ((char *) (a)))+16;
    printf("0x%llx\n0x%llx\n",b,b+3);
    ASAN_MEMPOOL_ALLOC((void *)a, 16);
    *((char *)a-1)='x';
    free(a);free(a);
    VALGRIND_MAKE_MEM_NOACCESS(a,1);
    printf("%d\n",__asan_address_is_poisoned(a));
    // VALGRIND_MAKE_MEM_DEFINED(a,1);
    // *(a)='x';
    free((void *)a);
    return 0;
}