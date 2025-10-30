
#include "rasp_context.h"

RASP_CONTEXT * create_rasp_context( RASP_PROGRAM * program, int num_registers ){
    RASP_CONTEXT * context = (RASP_CONTEXT*) safe_malloc( sizeof(RASP_CONTEXT) );
    context->registers = (int*) safe_malloc( sizeof(int)*num_registers );
    context->num_registers = num_registers;
    for(int i=0; i<num_registers; i++){
        context->registers[i] = 0;
    }
    context->program = program;
    context->program_counter = 0;
    context->accumulator = 0;
    context->last_instruction_result = safe_malloc( sizeof(RASP_INSTRUCTION_RESULT) );
    context->last_instruction_result->should_halt = 0;
    return context;
}