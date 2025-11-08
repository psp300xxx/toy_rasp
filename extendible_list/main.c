#include <stdio.h>
#include "extendible_list.h"

int main(void){
    EXTENDIBLE_LIST * extendible_list = create_extendible_list_int();
    free_extendible_list(extendible_list);
    return 0;
}