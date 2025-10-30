
#include <stdio.h>
#include "rasp_program/rasp_program.h"
#include "rasp_executor/rasp_context.h"

#define MAX_REGISTERS 16

int main(int argc, char ** argv){
    if(argc < 2){
        fprintf( stderr, "Usage: %s <rasp_program_file>\n", argv[0] );
        return 1;
    }
    const char * rasp_program_filepath = argv[1];
    RASP_PROGRAM * program = parse_file_to_rasp_program( rasp_program_filepath );
    if( program == NULL ){
        fprintf( stderr, "Error parsing RASP program from file: %s\n", rasp_program_filepath );
        return 1;
    }
    RASP_CONTEXT * context = create_rasp_context( program, MAX_REGISTERS );
    run_flawless( context, OPTION_MASK(SHOW_ACCUMULATOR) );
    free_rasp_context( context );
    return 0;          
}