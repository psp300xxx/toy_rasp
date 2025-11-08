
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include "../malloc_utils/malloc_utils.h"

#define ARRAY_LENGTH 128000

#define ENTRY_TYPE_INT 1
#define ENTRY_TYPE_VOID_POINTER 2
#define ENTRY_TYPE_STR 3

typedef struct {
    int type;
    union 
    {
        void * pointer;
        int integer;
        char * str;
        /* data */
    } data;
} HASHTABLE_ENTRY;

typedef struct {
    HASHTABLE_ENTRY * array [ARRAY_LENGTH];
    size_t size;
} HASHTABLE;

HASHTABLE * create_table();

/*
    I assume the user apply an hashing function to the object
    which returns key, in case of collision, the new value replaces the old one.
    Returns the old value associated with the key, or NULL if there was none.
*/
void put(unsigned int key, void * value, HASHTABLE * table);

/*
    I assume the user apply an hashing function to the object
    which returns key, in case of collision, the new value replaces the old one.
    Returns the old value associated with the key, or NULL if there was none.
*/
void putStrKey(char * strkey, void * value, HASHTABLE * table);

void put_integer(unsigned int key, int value, HASHTABLE * table);

void put_integerstrkey(char * strkey, int value, HASHTABLE * table);

void putstr_strkey(char * strkey, char * value, HASHTABLE * table);

void putstr(unsigned int key, char * value, HASHTABLE * table);

size_t size( HASHTABLE * table );

void for_each_key( HASHTABLE * table, void (*func)(unsigned int) );

void print_hashtable( HASHTABLE * table );

void for_each_entry( HASHTABLE * table, void (*func)(unsigned int, void *) );

void * get(unsigned int key, HASHTABLE * table); 

void * getstrkey(char * str, HASHTABLE * table); 

int get_integer(unsigned int key, HASHTABLE * table); 

int get_integerstrkey(char * str, HASHTABLE * table);

char * getstr_strkey(char * str, HASHTABLE * table);

char * getstr(unsigned int key, HASHTABLE * table);

void remove_key(unsigned int key, HASHTABLE * table);

void remove_keystrkey(char * str, HASHTABLE * table);

void free_table( HASHTABLE * table );

unsigned int hash_string( const char * str );

#endif /* HASHTABLE_H */
