
#ifndef MALLOC_UTILS_H
#define MALLOC_UTILS_H

#include <stdlib.h>

void * safe_malloc(size_t bytes);

void * safe_realloc(void * ptr, size_t bytes);

#endif /* MALLOC_UTILS_H */