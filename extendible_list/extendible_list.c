
#include "extendible_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

EXTENDIBLE_LIST * create_extendible_list_int(){
    EXTENDIBLE_LIST * list = (EXTENDIBLE_LIST*) safe_malloc( sizeof(EXTENDIBLE_LIST) );
    size_t elementSize = sizeof(int);
    list->type = ENTRY_TYPE_INT;
    list->data = safe_malloc( elementSize*ARRAY_START_LENGTH );
    list->size = 0;
    list->capacity = ARRAY_START_LENGTH;
    list->elementSize = elementSize;
    return list;
}

EXTENDIBLE_LIST * create_extendible_list_pointer(size_t elementSize){
    EXTENDIBLE_LIST * list = (EXTENDIBLE_LIST*) safe_malloc( sizeof(EXTENDIBLE_LIST) );
    list->type = ENTRY_TYPE_VOID_POINTER;
    list->data = safe_malloc( elementSize*ARRAY_START_LENGTH );
    list->size = 0;
    list->capacity = ARRAY_START_LENGTH;
    list->elementSize = elementSize;
    return list;
}

void free_extendible_list( EXTENDIBLE_LIST * list ){
    if( list==NULL ) return;
    if( list->data != NULL ){
        free( list->data );
    }
    free( list );
}

void extendible_list_add_int( EXTENDIBLE_LIST * list, int value ){
    if( list->type != ENTRY_TYPE_INT ){
        fprintf( stderr, "Error: trying to add int to non-int extendible list\n" );
        exit(1);
    }
    if( list->size == list->capacity ){
        list->capacity *= 2;
        list->data = safe_realloc( list->data, list->elementSize*list->capacity );
    }
    ((int*)list->data)[ list->size ] = value;
    list->size ++;
    increase_capacity_if_needed(list);
}

void extendible_list_add_pointer( EXTENDIBLE_LIST * list, void * value ){
    if( list->type != ENTRY_TYPE_VOID_POINTER ){
        fprintf( stderr, "Error: trying to add pointer to non-pointer extendible list\n" );
        exit(1);
    }
    if( list->size == list->capacity ){
        list->capacity *= 2;
        list->data = safe_realloc( list->data, list->elementSize*list->capacity );
    }
    char * charPtr = (char *) list->data;
    size_t offset = list->size * list->elementSize;
    char * targetPtr = (charPtr + offset);
    memcpy(targetPtr, list->elementSize, value);
    list->size ++;
    increase_capacity_if_needed(list);
}

int extendible_list_get_int( EXTENDIBLE_LIST * list, size_t index ){
    if( list->type != ENTRY_TYPE_INT ){
        fprintf( stderr, "Error: trying to get int from non-int extendible list\n" );
        exit(1);
    }
    if( index >= list->size || index<0 ){
        fprintf( stderr, "Error: index out of bounds in extendible list get_int\n" );
        exit(1);
    }
    return ((int*)list->data)[ index ];
}

void * extendible_list_get_pointer( EXTENDIBLE_LIST * list, size_t index ){
    if( list->type != ENTRY_TYPE_VOID_POINTER ){
        fprintf( stderr, "Error: trying to get pointer from non-pointer extendible list\n" );
        exit(1);
    }
    if( index >= list->size || index<0 ){
        fprintf( stderr, "Error: index out of bounds in extendible list get_pointer\n" );
        exit(1);
    }
    char * charPtr = (char *) list->data;
    size_t offset = index * list->elementSize;
    char * targetPtr = (charPtr + offset);
    return targetPtr;
}

size_t extendible_list_size( EXTENDIBLE_LIST * list ){
    return list->size;
}

int extendible_list_is_empty( EXTENDIBLE_LIST * list ){
    return list->size==0;
}

int extendible_list_contains_int( EXTENDIBLE_LIST * list, int value ){
    return extendible_list_contains_index_of(list, value) < 0 ? 0 : 1;
}

int extendible_list_contains_index_of( EXTENDIBLE_LIST * list, int value ){
    if(list->type!=ENTRY_TYPE_INT){
        fprintf(stderr, "Error: trying to look for int in a non-int extendible list\n");
        exit(1);
    }
    int * ptr = (int *)(list->data);
    for(size_t i = 0; i<list->size; i++){
        int v = ptr[i];
        if(v==value){
            return i;
        }
    }
    return -1;
}

/*
    Assumes the list is sorted in ascending order
    Returns the index of the value if found, -1 otherwise
*/
int extendible_list_index_of_binary_search( EXTENDIBLE_LIST * list, int value ){
    if(list->type!=ENTRY_TYPE_INT){
        fprintf(stderr, "Error: trying to look for int in a non-int extendible list\n");
        exit(1);
    }
    int * ptr = (int *)(list->data);
    int low = 0;
    int high = list->size;
    while(low<high){
        int mid = low + (high-low)/2;
        if( ptr[mid] == value ){
            return mid;
        }
        else if ( value > ptr[mid] ){
            low = mid+1;
        }
        else {
            high = mid;
        }
    }
    return -1;
}

int intcomparator(const void * a, const void * b){
    int int_a = *((int*) a);
    int int_b = *((int*) b);
    if( int_a < int_b ) return -1;
    if( int_a > int_b ) return 1;
    return 0;
}

void extendible_list_sort_ints( EXTENDIBLE_LIST * list ){
    if(list->type!=ENTRY_TYPE_INT){
        fprintf(stderr, "Error: trying to sort a non-int extendible list\n");
        exit(1);
    }
    int * ptr = (int *)(list->data);
    qsort( ptr, list->size, list->elementSize, (int (*)(const void *, const void *)) intcomparator );
}

void extendible_list_sort_pointers( EXTENDIBLE_LIST * list, int (*comparator)(const void *, const void *) ){
    if(list->type!=ENTRY_TYPE_VOID_POINTER){
        fprintf(stderr, "Error: trying to sort a non-pointer extendible list\n");
        exit(1);
    }
    qsort( list->data, list->size, list->elementSize, comparator );
}

int extendible_list_remove_int( EXTENDIBLE_LIST * list, int value ){
    if(list->type!=ENTRY_TYPE_INT){
        fprintf(stderr, "Error: trying to remove int from a non-int extendible list\n");
        exit(1);
    }
    int * ptr = (int *)(list->data);
    for(size_t i = 0; i<list->size; i++){
        int v = ptr[i];
        if(v==value){
            // Shift elements to the left
            for(size_t j=i; j<list->size-1; j++){
                ptr[j] = ptr[j+1];
            }
            list->size --;
            return 1; // Successfully removed
        }
    }
    decrease_capacity_if_needed(list);
    return 0; // Value not found
}

int extendible_list_remove_int_at( EXTENDIBLE_LIST * list, int index ){
    if(list->type!=ENTRY_TYPE_INT){
        fprintf(stderr, "Error: trying to remove int from a non-int extendible list\n");
        exit(1);
    }
    if( index < 0 || index >= list->size ){
        fprintf(stderr, "Error: index out of bounds in extendible_list_remove_int_at\n");
        exit(1);
    }
    int * ptr = (int *)(list->data);
    // Shift elements to the left
    for(size_t j=index; j<list->size-1; j++){
        ptr[j] = ptr[j+1];
    }
    list->size --;
    decrease_capacity_if_needed(list);
    return 1; // Successfully removed
}

void decrease_capacity_if_needed(EXTENDIBLE_LIST *list) {
    if (list->size > list->capacity / 4 || list->capacity <= ARRAY_START_LENGTH) {
        return;
    }
    list->capacity /= 2;
    list->data = safe_realloc(list->data, list->elementSize * list->capacity);
}


void extendible_list_remove_pointer_at( EXTENDIBLE_LIST * list, size_t index ){
    if(list->type!=ENTRY_TYPE_VOID_POINTER){
        fprintf(stderr, "Error: trying to remove pointer from a non-pointer extendible list\n");
        exit(1);
    }
    if( index < 0 || index >= list->size ){
        fprintf(stderr, "Error: index out of bounds in extendible_list_remove_pointer_at\n");
        exit(1);
    }
    char * ptr = (char *)(list->data);
    size_t offset = (index * list->elementSize);
    char * targetPtr = (ptr + offset);
    int idx = index;
    while (idx < list->size){
        memcpy( targetPtr, list->elementSize, targetPtr + list->elementSize );
        idx++;
        targetPtr = targetPtr + list->elementSize;
    }
    list->size --;
    decrease_capacity_if_needed(list);
}

void increase_capacity_if_needed(EXTENDIBLE_LIST *list){
    if(list->size < list->capacity){
        return;
    }
    list->capacity = 2*list->capacity;
    list->data = safe_realloc(list->data, list->capacity*list->elementSize);
}

void extendible_list_invert_inplace(EXTENDIBLE_LIST *list){
    size_t left = 0;
    size_t right = list->size - 1;
    if(list->type == ENTRY_TYPE_INT){
        int * dataAsIntArray = (int *) list->data;
        while (left < right) {
            int temp = dataAsIntArray[left];
            dataAsIntArray[left] = dataAsIntArray[right];
            dataAsIntArray[right] = temp;
            left++;
            right--;
        }
    } else if (list->type == ENTRY_TYPE_VOID_POINTER){
        char * tmpptr = safe_malloc( list->elementSize );
        while (left < right) {
            size_t leftOffset = list->elementSize * left;
            size_t rightOffset = list->elementSize * right;
            char * leftptr = (char * ) list->data + leftOffset;
            char * rightptr = (char *) list->data + rightOffset;
            memcpy(tmpptr, list->elementSize, leftptr);
            memcpy(leftptr, list->elementSize, rightptr);
            memcpy(rightptr, list->elementSize, tmpptr);
            left++;
            right--;
        }
        free(tmpptr);
    } else {
        fprintf(stderr, "Error: unknown extendible list type in invert_inplace\n");
        exit(1);
    }
}

EXTENDIBLE_LIST * extendible_list_inverted(EXTENDIBLE_LIST *list){
    EXTENDIBLE_LIST * invertedList = safe_malloc( sizeof(EXTENDIBLE_LIST) );
    invertedList->size = 0;
    invertedList->capacity = list->capacity;
    invertedList->elementSize = list->elementSize;
    invertedList->type = list->type;
    invertedList->data = safe_malloc( list->elementSize * list->capacity );
    if(list->type == ENTRY_TYPE_INT){
        invertedList = create_extendible_list_int();
        for(size_t i = list->size; i>0; i--){
            int value = extendible_list_get_int(list, i-1);
            extendible_list_add_int(invertedList, value);
        }
    } else if (list->type == ENTRY_TYPE_VOID_POINTER){
        invertedList = create_extendible_list_pointer(list->elementSize);
        for(size_t i = list->size; i>0; i--){
            void * value = extendible_list_get_pointer(list, i-1);
            extendible_list_add_pointer(invertedList, value);
        }
    } else {
        fprintf(stderr, "Error: unknown extendible list type in inverted\n");
        free_extendible_list(invertedList);
        return NULL;
    }
    return invertedList;
}