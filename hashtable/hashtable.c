
#include "hashtable.h"
#include <limits.h>
#include <stdio.h>


HASHTABLE * create_table(){
    HASHTABLE * table = (HASHTABLE*) safe_malloc( sizeof(HASHTABLE) );
    table->size = 0;
    for(int i=0; i<ARRAY_LENGTH; i++){
        table->array[i] = NULL;
    }
    return table;
}

void put(unsigned int key, void * value, HASHTABLE * table){
    void * old_value =  table->array[ key%ARRAY_LENGTH ];
    table->array[ key%ARRAY_LENGTH ] = safe_malloc( sizeof(HASHTABLE_ENTRY) );
    table->size += (old_value==NULL) ? 1 : 0;
    if( old_value != NULL ){
        free( old_value );
    }
    HASHTABLE_ENTRY * entry = (HASHTABLE_ENTRY*) table->array[ key%ARRAY_LENGTH ];
    entry->type = ENTRY_TYPE_VOID_POINTER;
    entry->data.pointer = value;
}

void put_integer(unsigned int key, int value, HASHTABLE * table){
    void * old_value =  table->array[ key%ARRAY_LENGTH ];
    table->array[ key%ARRAY_LENGTH ] = safe_malloc( sizeof(HASHTABLE_ENTRY) );
    HASHTABLE_ENTRY * entry = (HASHTABLE_ENTRY*) table->array[ key%ARRAY_LENGTH ];
    entry->type = ENTRY_TYPE_INT;
    entry->data.integer = value;
    table->size += (old_value==NULL) ? 1 : 0;
    if( old_value != NULL ){
        free( old_value );
    }
}

void putstr_strkey(char * strkey, char * value, HASHTABLE * table){
    unsigned int hash = hash_string( strkey );
    putstr( hash, value, table );
}

void putstr(unsigned int key, char * value, HASHTABLE * table){
    void * old_value =  table->array[ key%ARRAY_LENGTH ];
    table->array[ key%ARRAY_LENGTH ] = safe_malloc( sizeof(HASHTABLE_ENTRY) );
    HASHTABLE_ENTRY * entry = (HASHTABLE_ENTRY*) table->array[ key%ARRAY_LENGTH ];
    entry->type = ENTRY_TYPE_STR;
    entry->data.str = value;
    table->size += (old_value==NULL) ? 1 : 0;
    if( old_value != NULL ){
        free( old_value );
    }
}

void putStrKey(char * strkey, void * value, HASHTABLE * table){
    unsigned int hash = hash_string( strkey );
    put(hash, value, table);
}

void print_hashtable( HASHTABLE * table ){
    printf("Hashtable contents:\n");
    for(int i=0; i<ARRAY_LENGTH; i++){
        HASHTABLE_ENTRY * entry = table->array[i];
        if( entry != NULL ){
            switch ( entry->type )
            {
                case ENTRY_TYPE_INT:
                    printf("Key: %d, Value (int): %d\n", i, entry->data.integer );
                    break;
                case ENTRY_TYPE_VOID_POINTER:
                    printf("Key: %d, Value (pointer): %p\n", i, entry->data.pointer );
                    break;
                case ENTRY_TYPE_STR:
                    printf("Key: %d, Value (str): %s\n", i, entry->data.str );
                    break;
                default:
                    printf("Key: %d, Value: Unknown type\n", i );
                    break;
            }
        }
    }
}

void put_integerstrkey(char * strkey, int value, HASHTABLE * table){
    unsigned int hash = hash_string( strkey );
    put_integer(hash, value, table);
}

void * getstrkey(char * str, HASHTABLE * table){
    unsigned int hash = hash_string( str );
    return get( hash, table );
}

char * getstr(unsigned int key, HASHTABLE * table){
    HASHTABLE_ENTRY * entry = table->array[ key%ARRAY_LENGTH ];
    if( entry == NULL ){
        return NULL;
    }
    if( entry->type != ENTRY_TYPE_STR ){
        return NULL; // Or handle type mismatch as needed
    }
    return entry->data.str;
}

char * getstr_strkey(char * str, HASHTABLE * table){
    unsigned int hash = hash_string( str );
    return getstr( hash, table );
}

int get_integerstrkey(char * str, HASHTABLE * table){
    unsigned int hash = hash_string( str );
    return get_integer( hash, table );
}

void * get(unsigned int key, HASHTABLE * table){
    HASHTABLE_ENTRY * entry = table->array[ key%ARRAY_LENGTH ];
    if( entry == NULL ){
        return NULL;
    }
    switch ( entry->type )
    {
        case ENTRY_TYPE_INT:
            return &entry->data.integer;
        case ENTRY_TYPE_VOID_POINTER:
            return entry->data.pointer;
        default:
            return NULL;
    }
}

int get_integer(unsigned int key, HASHTABLE * table){
    if( table->array[ key%ARRAY_LENGTH ] == NULL ){
        return INT_MAX; // Or some other sentinel value indicating absence
    }
    HASHTABLE_ENTRY * ptr = (HASHTABLE_ENTRY*) table->array[ key%ARRAY_LENGTH ];
    if( ptr->type != ENTRY_TYPE_INT ){
        return INT_MAX; // Or handle type mismatch as needed
    }
    return ptr->data.integer;
}

void remove_key(unsigned int key, HASHTABLE * table){
    HASHTABLE_ENTRY * old_value = table->array[ key%ARRAY_LENGTH ];
    if( old_value != NULL ){
        table->array[ key%ARRAY_LENGTH ] = NULL;
        table->size--;
        free( old_value );
    }
}

void remove_keystrkey(char * str, HASHTABLE * table){
    unsigned int hash = hash_string( str );
    remove_key( hash, table );
}

size_t size( HASHTABLE * table ){
    return table->size;
}

unsigned int hash_string( const char * str ){
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

void for_each_key( HASHTABLE * table, void (*func)(unsigned int) ){
    for(int i=0; i<ARRAY_LENGTH; i++){
        if( table->array[i] != NULL ){
            func(i);
        }
    }
}

void for_each_entry( HASHTABLE * table, void (*func)(unsigned int, void *) ){
    for(int i=0; i<ARRAY_LENGTH; i++){
        if( table->array[i] != NULL ){
            func(i, table->array[i]);
        }
    }
}

void free_table( HASHTABLE * table ){
    free( table );
}