#include "rasp_program.h"
#include <stdio.h>
#include "rasp_instruction.h"

RASP_PROGRAM * create_rasp_program( RASP_INSTRUCTION * instructions, HASHTABLE * labels ){
    RASP_PROGRAM * program = (RASP_PROGRAM*) safe_malloc( sizeof(RASP_PROGRAM) );
    program->instructions = instructions;
    program->labels = labels;
    return program;
}

RASP_PROGRAM * create_rasp_program_skeleton( int expected_instruction_number ){
    RASP_PROGRAM * program = (RASP_PROGRAM*) safe_malloc( sizeof(RASP_PROGRAM) );
    program -> instruction_count = 0;
    program->instructions = safe_malloc( sizeof(RASP_INSTRUCTION)*expected_instruction_number );
    program->labels = create_table();
    return program;
}

void add_instruction_to_rasp_program( RASP_INSTRUCTION * instruction, RASP_PROGRAM * program ){
    program->instructions[ program->instruction_count ] = *instruction;
    program->instruction_count ++;
    if( program->instruction_count == program ->expected_instruction_count-1 ){
        program->expected_instruction_count *= 2;
        program->instructions = (RASP_INSTRUCTION *) realloc( program->instructions, sizeof(RASP_INSTRUCTION)*(program->expected_instruction_count) );
    }
}

RASP_PROGRAM * parse_file_to_rasp_program( const char * filepath ){
    FILE * file = fopen( filepath, "r" );
    RASP_PROGRAM * program = (RASP_PROGRAM *)safe_malloc( sizeof(RASP_PROGRAM) );
    int ERR_PROGRAM_NOT_CREATED = 0;
    if( file==NULL ){
        fprintf( stderr, "error opening file %s\n", filepath );
        ERR_PROGRAM_NOT_CREATED = 1;
        goto finally;
    }
    int expected_instruction_count = 128;
    int max_line_length = 256;
    char * line = (char *) safe_malloc( sizeof(char)*max_line_length );
    ssize_t read;
    while ((read = getline(line, (size_t *)&max_line_length, file)) != -1) {
        RASP_INSTRUCTION * instruction = parse_rasp_instruction( line );
        if (instruction==NULL)
        {
            ERR_PROGRAM_NOT_CREATED = 1;
            fprintf( stderr, "error parsing instruction: %s\n", line );
            goto finally;
        }
        add_instruction_to_rasp_program( instruction, program );
        free( instruction );
    }
    for(int i=0; i<program->instruction_count; i++){
        RASP_INSTRUCTION * instr = &program->instructions[i];
        if( instr->label != NULL ){
            unsigned int label_hash = rasp_instruction_label_hash( instr->label );
            put( label_hash, (void *)(long)i, program->labels );
        }
    }
    finally:
        if(ERR_PROGRAM_NOT_CREATED && program!=NULL){
            // free program
            free_rasp_program( program );
            program = NULL;
        }
        if(file!=NULL){
            fclose( file );
        }
        return program;
}