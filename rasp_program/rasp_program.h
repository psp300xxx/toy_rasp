#ifndef RASP_PROGRAM_H
#define RASP_PROGRAM_H

#include "../hashtable/hashtable.h"
#include "../rasp_instruction/rasp_instruction.h"

typedef struct rasp_program {
    RASP_INSTRUCTION * instructions;
    HASHTABLE * labels;
    int instruction_count;
    int expected_instruction_count;
} RASP_PROGRAM;

RASP_PROGRAM * create_rasp_program( RASP_INSTRUCTION * instructions, HASHTABLE * labels );

RASP_PROGRAM * create_rasp_program_skeleton( int expected_instruction_number );

RASP_PROGRAM * parse_file_to_rasp_program( const char * filepath );

void add_instruction_to_rasp_program( RASP_INSTRUCTION * instruction, RASP_PROGRAM * program );

void free_rasp_program( RASP_PROGRAM * program );

void write_rasp_program_to_file( RASP_PROGRAM * program, const char * filepath );

#endif /* RASP_PROGRAM_H */