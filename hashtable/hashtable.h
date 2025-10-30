#include <stdlib.h>
#include "malloc_utils.h"

#define ARRAY_LENGTH 128000

typedef struct {
    void * array [ARRAY_LENGTH];
    unsigned int size;
} HASHTABLE;

HASHTABLE * create_table();

/*
    I assume the user apply an hashing function to the object
    which returns key, in case of collision, the new value replaces the old one.
    Returns the old value associated with the key, or NULL if there was none.
*/
void * put(int key, void * value, HASHTABLE * table);

int size( HASHTABLE * table );

void for_each_key( HASHTABLE * table, void (*func)(unsigned int) );

void for_each_entry( HASHTABLE * table, void (*func)(unsigned int, void *) );

void * get(unsigned int key, HASHTABLE * table);

void * remove(unsigned int key, HASHTABLE * table);

void free_table( HASHTABLE * table );
