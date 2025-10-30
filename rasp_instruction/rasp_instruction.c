#include "rasp_instruction.h"
#include "rasp_context.h"
#include <unistd.h> 

void rasp_jump(int typeOfOperand, int operand, RASP_CONTEXT * context){
    context->program_counter = operand;
}

void rasp_load(int typeOfOperand, int operand, RASP_CONTEXT * context){
    switch( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            context->accumulator = context->registers[ operand ];
            break;
        case RASP_OPERAND_FROM_RAW_VALUE:
            context->accumulator = operand;
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            context->accumulator = context->registers[ context->registers[ operand ] ];
            break;
    }
    context->program_counter ++;
}

void rasp_store(int typeOfOperand, int operand, RASP_CONTEXT * context){
    context->registers[ operand ] = context->accumulator;
    context->program_counter ++;
}

void rasp_add(int typeOfOperand, int operand, RASP_CONTEXT * context){
    switch( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            context->accumulator += context->registers[ operand ];
            break;
        case RASP_OPERAND_FROM_RAW_VALUE:
            context->accumulator += operand;
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            context->accumulator += context->registers[ context->registers[ operand ] ];
            break;
    }
    context->program_counter ++;
}

void rasp_sub(int typeOfOperand, int operand, RASP_CONTEXT * context){
    switch( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            context->accumulator -= context->registers[ operand ];
            break;
        case RASP_OPERAND_FROM_RAW_VALUE:
            context->accumulator -= operand;
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            context->accumulator -= context->registers[ context->registers[ operand ] ];
            break;
    }
    context->program_counter ++;
}

void rasp_mul(int typeOfOperand, int operand, RASP_CONTEXT * context){
    switch( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            context->accumulator *= context->registers[ operand ];
            break;
        case RASP_OPERAND_FROM_RAW_VALUE:
            context->accumulator *= operand;
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            context->accumulator *= context->registers[ context->registers[ operand ] ];
            break;
    }
    context->program_counter ++;
}

void rasp_div(int typeOfOperand, int operand, RASP_CONTEXT * context){
    switch( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            context->accumulator /= context->registers[ operand ];
            break;
        case RASP_OPERAND_FROM_RAW_VALUE:
            context->accumulator /= operand;
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            context->accumulator /= context->registers[ context->registers[ operand ] ];
            break;
    }
    context->program_counter ++;
}

void rasp_noop(int typeOfOperand, int operand, RASP_CONTEXT * context){
    context->program_counter ++;
}   

void rasp_jump_if_zero(int typeOfOperand, int operand, RASP_CONTEXT * context){
    if( context->accumulator == 0 ){
        context->program_counter = operand;
        return;
    }
    context->program_counter ++;
}

void rasp_halt(int typeOfOperand, int operand, RASP_CONTEXT * context){
    context->last_instruction_result->should_halt = 1;
    printf("HALT encountered. Stopping execution.\n");
    exit(0);
}

void rasp_jump_if_greater(int typeOfOperand, int operand, RASP_CONTEXT * context){
    if( context->accumulator > 0 ){
        context->program_counter = operand;
        return;
    }
    context->program_counter ++;
}

void rasp_jump_if_not_zero(int typeOfOperand, int operand, RASP_CONTEXT * context){
    if( context->accumulator != 0 ){
        context->program_counter = operand;
        return;
    }
    context->program_counter ++;
}

void rasp_jump_if_greater_or_equals(int typeOfOperand, int operand, RASP_CONTEXT * context){
    if( context->accumulator >= 0 ){
        context->program_counter = operand;
        return;
    }
    context->program_counter ++;
}

void execute_rasp_instruction( RASP_INSTRUCTION * instruction, RASP_CONTEXT * context, RASP_INSTRUCTION_RESULT * result ){
    int opcode = instruction->opcode;
    RASP_FUNCTIONS[ opcode ]( instruction->typeOfOperand, instruction->operand, context ); 
}

void rasp_jump_if_less(int typeOfOperand, int operand, RASP_CONTEXT * context){
    if( context->accumulator < 0 ){
        context->program_counter = operand;
        return;
    }
    context->program_counter ++;
}

void rasp_jump_if_less_or_equals(int typeOfOperand, int operand, RASP_CONTEXT * context){
    if( context->accumulator <= 0 ){
        context->program_counter = operand;
        return;
    }
    context->program_counter ++;
}

void rasp_print(int typeOfOperand, int operand, RASP_CONTEXT * context){
    printf("PRINT: %d\n", context->accumulator);
    context->program_counter ++;
}


RASP_INSTRUCTION * parse_rasp_instruction( char * line ){

}

unsigned int rasp_instruction_label_hash( char * label ){
    unsigned int hash = 5381;
    int c;
    while ( (c = *label++) ) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}