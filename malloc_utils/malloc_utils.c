#include "malloc_utils.h"
#include <unistd.h>
#include <stdio.h>

void * safe_malloc(size_t bytes){
    void * result = malloc(bytes);
    if( result==NULL ){
        fprintf(stderr, "error malloc'ing %zu bytes\n", bytes);
        exit(1);
    }
    return result;
}