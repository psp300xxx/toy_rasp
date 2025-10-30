
#include "hashtable.h"

HASHTABLE * create_table(){
    HASHTABLE * table = (HASHTABLE*) safe_malloc( sizeof(HASHTABLE) );
    table->size = 0;
    for(int i=0; i<ARRAY_LENGTH; i++){
        table->array[i] = NULL;
    }
    return table;
}

void * put(int key, void * value, HASHTABLE * table){
    void * old_value =  table->array[ key%ARRAY_LENGTH ];
    table->array[ key%ARRAY_LENGTH ] = value;
    table->size += (old_value==NULL) ? 1 : 0;
    return old_value;
}

void * get(unsigned int key, HASHTABLE * table){
    return table->array[ key%ARRAY_LENGTH ];
}

void * remove(unsigned int key, HASHTABLE * table){
    void * old_value = table->array[ key%ARRAY_LENGTH ];
    if( old_value != NULL ){
        table->array[ key%ARRAY_LENGTH ] = NULL;
        table->size--;
    }
    return old_value;
}

int size( HASHTABLE * table ){
    return table->size;
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