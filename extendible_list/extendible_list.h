
#ifndef EXTENDIBLE_LIST_H
#define EXTENDIBLE_LIST_H

#include <stdlib.h>
#include "../malloc_utils/malloc_utils.h"

#define ARRAY_START_LENGTH 64

#define ENTRY_TYPE_INT 1
#define ENTRY_TYPE_VOID_POINTER 2

typedef struct {
    int type;
    void * data;
    size_t size;
    size_t capacity;
    size_t elementSize;
} EXTENDIBLE_LIST;

EXTENDIBLE_LIST * create_extendible_list_int();

int intcomparator(const void * a, const void * b);

EXTENDIBLE_LIST * create_extendible_list_pointer(size_t elementSize);

void free_extendible_list( EXTENDIBLE_LIST * list );

void extendible_list_add_int( EXTENDIBLE_LIST * list, int value );

void extendible_list_add_pointer( EXTENDIBLE_LIST * list, void * value );

void decrease_capacity_if_needed(EXTENDIBLE_LIST *list);

void increase_capacity_if_needed(EXTENDIBLE_LIST *list);

void extendible_list_invert_inplace(EXTENDIBLE_LIST *list);

EXTENDIBLE_LIST * extendible_list_inverted(EXTENDIBLE_LIST *list);

int extendible_list_get_int( EXTENDIBLE_LIST * list, size_t index );

void * extendible_list_get_pointer( EXTENDIBLE_LIST * list, size_t index );

size_t extendible_list_size( EXTENDIBLE_LIST * list );

int extendible_list_is_empty( EXTENDIBLE_LIST * list );

int extendible_list_contains_int( EXTENDIBLE_LIST * list, int value );

int extendible_list_contains_index_of( EXTENDIBLE_LIST * list, int value );

/*
    Assumes the list is sorted in ascending order
    Returns the index of the value if found, -1 otherwise
*/
int extendible_list_index_of_binary_search( EXTENDIBLE_LIST * list, int value );

void extendible_list_sort_ints( EXTENDIBLE_LIST * list );

void extendible_list_sort_pointers( EXTENDIBLE_LIST * list, int (*comparator)(const void *, const void *) );

int extendible_list_remove_int( EXTENDIBLE_LIST * list, int value );

int extendible_list_remove_int_at( EXTENDIBLE_LIST * list, int index );

void extendible_list_remove_pointer_at( EXTENDIBLE_LIST * list, size_t index );

#endif /* EXTENDIBLE_LIST_H */
