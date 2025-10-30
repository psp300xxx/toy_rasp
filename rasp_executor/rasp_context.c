
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

int should_halt(RASP_CONTEXT * context){
    return context->last_instruction_result->should_halt;
}

void advance(RASP_CONTEXT * context){
    execute_rasp_instruction( &context->program->instructions[ context->program_counter ], context, context->last_instruction_result );
}

void run_flawless(RASP_CONTEXT * context, int OPTIONS ){
    while( !should_halt(context) ){
        advance(context);
        if( OPT_ENABLED( OPTIONS, SHOW_DEBUG_OUTPUT ) ){
            print_context_state( context );
        }
        if( OPT_ENABLED( OPTIONS, SHOW_ACCUMULATOR ) ){
            printf("ACC: %d\n", context->accumulator );
        }
    }
}

void print_context_state(RASP_CONTEXT * context){
    printf("PC: %d, ACC: %d\n", context->program_counter, context->accumulator );
    for(int i=0; i<context->num_registers; i++){
        printf("R%d: %d ", i, context->registers[i] );
    }
    printf("\n\n");
}

void free_rasp_context( RASP_CONTEXT * context ){
    if( context==NULL ) return;
    if( context->registers != NULL ){
        free( context->registers );
    }
    if( context->last_instruction_result != NULL ){
        free( context->last_instruction_result );
    }
    free( context );
}
