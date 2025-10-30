
#ifndef RASP_CONTEXT_H
#define RASP_CONTEXT_H

#include "../rasp_program/rasp_program.h"

typedef struct RASP_CONTEXT {
    int * registers;
    int num_registers;
    RASP_PROGRAM * program;
    int program_counter;
    int accumulator;
    RASP_INSTRUCTION_RESULT * last_instruction_result;
} RASP_CONTEXT;


RASP_CONTEXT * create_rasp_context( RASP_PROGRAM * program, int num_registers );

#endif /* RASP_CONTEXT_H */

