#include "rasp_program.h"
#include <stdio.h>
#include "../rasp_instruction/rasp_instruction.h"
#include <limits.h>

#define UNUSED(x) (void)(x)

RASP_PROGRAM * create_rasp_program( RASP_INSTRUCTION * instructions, HASHTABLE * labels ){
    RASP_PROGRAM * program = (RASP_PROGRAM*) safe_malloc( sizeof(RASP_PROGRAM) );
    program->instructions = instructions;
    program->labels = labels;
    return program;
}

void free_rasp_program( RASP_PROGRAM * program ){
    if( program==NULL ) return;
    if( program->instructions != NULL ){
        free( program->instructions );
    }
    if( program->labels != NULL ){
        free_table( program->labels );
    }
    free( program );
}

RASP_PROGRAM * create_rasp_program_skeleton( int expected_instruction_number ){
    RASP_PROGRAM * program = (RASP_PROGRAM*) safe_malloc( sizeof(RASP_PROGRAM) );
    program -> instruction_count = 0;
    program->instructions = safe_malloc( sizeof(RASP_INSTRUCTION)*expected_instruction_number );
    program->labels = create_table();
    program->expected_instruction_count = expected_instruction_number;
    return program;
}

void add_instruction_to_rasp_program( RASP_INSTRUCTION * instruction, RASP_PROGRAM * program ){
    program->instructions[ program->instruction_count ] = *instruction;
    program->instruction_count ++;
    if( program->instruction_count == program ->expected_instruction_count-1 ){
        program->expected_instruction_count *= 2;
        program->instructions = (RASP_INSTRUCTION *) safe_realloc( program->instructions, sizeof(RASP_INSTRUCTION)*(program->expected_instruction_count) );
    }
}

RASP_PROGRAM * parse_file_to_rasp_program( const char * filepath ){
    FILE * file = fopen( filepath, "r" );
    int ERR_PROGRAM_NOT_CREATED = 0;
    size_t current_line = 1;
    char * line = NULL;
    if( file==NULL ){
        fprintf( stderr, "error opening file %s\n", filepath );
        ERR_PROGRAM_NOT_CREATED = 1;
        goto finally;
    }
    int expected_instruction_count = 128;
    RASP_PROGRAM * program = create_rasp_program_skeleton( expected_instruction_count );
    size_t max_line_length = 0;
    ssize_t read;
    while ((read = getline(&line, &max_line_length, file)) != -1) {
        RASP_INSTRUCTION * instruction = parse_rasp_instruction( line, current_line++ );
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
            put_integerstrkey( instr->label, i, program->labels );
        }
    }
    for(int i=0; i<program->instruction_count; i++){
        RASP_INSTRUCTION * instr = &program->instructions[i];
        if( is_jump_opcode( instr->opcode ) ){
            int label_address = get_integerstrkey( instr->targetLabel, program->labels );
            if( label_address == INT_MAX ){
                ERR_PROGRAM_NOT_CREATED = 1;
                fprintf( stderr, "error: label not found for instruction at index %d\n", i );
                goto finally;
            }
            instr->operand = label_address;
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
        if(line!=NULL){
            free(line);
        }
        return program;
}

void write_rasp_program_to_file( RASP_PROGRAM * program, const char * filepath ){
   UNUSED(program);
   UNUSED(filepath); 
}