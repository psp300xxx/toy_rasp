
#ifndef RASP_CONTEXT_H
#define RASP_CONTEXT_H

#define SHOW_DEBUG_OUTPUT 0

#define SHOW_ACCUMULATOR 1

#define OPTION_MASK(x) (1 << x)

#define OPT_ENABLED(val, bit) ( (( val >> bit ) & 1) == 1 )

#include "../rasp_program/rasp_program.h"
#include "../hashtable/hashtable.h"

typedef struct RASP_CONTEXT {
    int * registers;
    int num_registers;
    RASP_PROGRAM * program;
    int program_counter;
    int accumulator;
    HASHTABLE * etiquettes;
    RASP_INSTRUCTION_RESULT * last_instruction_result;
} RASP_CONTEXT;


RASP_CONTEXT * create_rasp_context( RASP_PROGRAM * program, int num_registers );

int should_halt(RASP_CONTEXT * context);

void advance(RASP_CONTEXT * context);

void run_flawless(RASP_CONTEXT * context, int OPTIONS);

void free_rasp_context( RASP_CONTEXT * context );

void print_context_state(RASP_CONTEXT * context);

#endif /* RASP_CONTEXT_H */

