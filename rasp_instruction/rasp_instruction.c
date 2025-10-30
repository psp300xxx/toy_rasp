#include "rasp_instruction.h"
#include "../rasp_executor/rasp_context.h"
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#define UNUSED(x) (void)(x)


/* Define the function pointer table in the implementation file to avoid
   multiple-definition linker errors. The header contains an extern
   declaration for this symbol. */
void (*RASP_FUNCTIONS[RASP_HALT + 1])(int typeOfOperand, int operand, RASP_CONTEXT * context) = {
    rasp_noop,
    rasp_load,
    rasp_store,
    rasp_add,
    rasp_sub,
    rasp_mul,
    rasp_mod,
    rasp_inc,
    rasp_div,
    rasp_jump,
    rasp_jump_if_zero,
    rasp_jump_if_not_zero,
    rasp_jump_if_greater,
    rasp_jump_if_greater_or_equals,
    rasp_jump_if_less,
    rasp_jump_if_less_or_equals,
    rasp_print,
    rasp_halt
};

void rasp_jump(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
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
    increase_program_counter( context );
}

void rasp_store(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
    context->registers[ operand ] = context->accumulator;
    increase_program_counter( context );
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
    increase_program_counter( context );
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
    increase_program_counter( context );
}

void rasp_mod(int typeOfOperand, int operand, RASP_CONTEXT * context){
    switch( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            context->accumulator %= context->registers[ operand ];
            break;
        case RASP_OPERAND_FROM_RAW_VALUE:
            context->accumulator %= operand;
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            context->accumulator %= context->registers[ context->registers[ operand ] ];
            break;
    }
    increase_program_counter( context );
}

void rasp_inc(int typeOfOperand, int operand, RASP_CONTEXT * context){
    switch( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            context->registers[ operand ]++;
            break;
        default:
            context->accumulator++;
            break;
    }
    increase_program_counter( context );
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
    increase_program_counter( context );
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
    increase_program_counter( context );
}

void rasp_noop(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
    UNUSED(operand);
    increase_program_counter( context );
}   

void rasp_jump_if_zero(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
    if( context->accumulator == 0 ){
        context->program_counter = operand;
        return;
    }
    context->program_counter ++;
}

void rasp_halt(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
    UNUSED(operand);
    context->last_instruction_result->should_halt = 1;
    printf("HALT encountered. Stopping execution.\n");
    exit(0);
}

void rasp_jump_if_greater(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
    if( context->accumulator > 0 ){
        context->program_counter = operand;
        return;
    }
    context->program_counter ++;
}

void rasp_jump_if_not_zero(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
    if( context->accumulator != 0 ){
        context->program_counter = operand;
        return;
    }
    context->program_counter ++;
}

int is_jump_opcode(int opcode){
    return ( opcode == RASP_JUMP ||
             opcode == RASP_JUMP_IF_ZERO ||
             opcode == RASP_JUMP_IF_NOT_ZERO ||
             opcode == RASP_JUMP_IF_GREATER ||
             opcode == RASP_JUMP_IF_GREATER_OR_EQUALS ||
             opcode == RASP_JUMP_IF_LESS ||
             opcode == RASP_JUMP_IF_LESS_OR_EQUALS );
}

void increase_program_counter(RASP_CONTEXT * context){
    context->program_counter ++;
    if( context->program_counter >= context->program->instruction_count ){
        context->last_instruction_result->should_halt = 1;
    }
}

void rasp_jump_if_greater_or_equals(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
    if( context->accumulator >= 0 ){
        context->program_counter = operand;
        return;
    }
    increase_program_counter( context );
}

void execute_rasp_instruction( RASP_INSTRUCTION * instruction, RASP_CONTEXT * context, RASP_INSTRUCTION_RESULT * result ){
    int opcode = instruction->opcode;
    RASP_FUNCTIONS[ opcode ]( instruction->typeOfOperand, instruction->operand, context ); 
    result->should_halt = context->last_instruction_result->should_halt;
}

void rasp_jump_if_less(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
    if( context->accumulator < 0 ){
        context->program_counter = operand;
        return;
    }
    increase_program_counter( context );
}

void rasp_jump_if_less_or_equals(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
    if( context->accumulator <= 0 ){
        context->program_counter = operand;
        return;
    }
    increase_program_counter( context );
}

void rasp_print(int typeOfOperand, int operand, RASP_CONTEXT * context){
    UNUSED(typeOfOperand);
    UNUSED(operand);
    printf("%d\n", context->accumulator);
    increase_program_counter( context );
}


RASP_INSTRUCTION * parse_rasp_instruction( char * line ){
    char label[64] = "";
    char instruction[64] = "";
    char operand[64] = "";

    // Rimuove spazi iniziali
    while (isspace((unsigned char)*line)) line++;

    // Verifica se contiene ':'
    if (strchr(line, ':')) {
        // LABEL: INSTRUCTION OPERAND
        sscanf(line, "%63[^:]: %63s %63s", label, instruction, operand);
    } else {
        // INSTRUCTION OPERAND
        sscanf(line, "%63s %63s", instruction, operand);
    }
    RASP_INSTRUCTION * rasp_instruction = (RASP_INSTRUCTION *) safe_malloc( sizeof(RASP_INSTRUCTION) );
    copy_label_if_non_empty(rasp_instruction, label);
    rasp_instruction->opcode = get_opcode_from_string( instruction );
    if( rasp_instruction->opcode == -1 ){
        free( rasp_instruction );
        fprintf( stderr, "Invalid instruction: %s\n", instruction );
        exit(1); // Invalid instruction
    }
    rasp_instruction->typeOfOperand = get_type_of_operand_from_string( operand );
    set_operand_from_string( rasp_instruction, rasp_instruction->opcode, operand );
    return rasp_instruction;
}

void copy_label_if_non_empty(RASP_INSTRUCTION * rasp_instruction, char * label) {
    if (strlen(label) > 0) {
        rasp_instruction->label = (char *) safe_malloc(strlen(label) + 1);
        strcpy(rasp_instruction->label, label);
    } else {
        rasp_instruction->label = NULL;
    }
}

int get_opcode_from_string( char * instruction_str ){
    if( strcmp(instruction_str, "NOOP")==0 ) return RASP_NOOP;
    if( strcmp(instruction_str, "LOAD")==0 ) return RASP_LOAD;
    if( strcmp(instruction_str, "STORE")==0 ) return RASP_STORE;
    if( strcmp(instruction_str, "ADD")==0 ) return RASP_ADD;
    if( strcmp(instruction_str, "SUB")==0 ) return RASP_SUB;
    if( strcmp(instruction_str, "MUL")==0 ) return RASP_MUL;
    if( strcmp(instruction_str, "MOD")==0 ) return RASP_MOD;
    if( strcmp(instruction_str, "INC")==0 ) return RASP_INC;
    if( strcmp(instruction_str, "DIV")==0 ) return RASP_DIV;
    if( strcmp(instruction_str, "JUMP")==0 ) return RASP_JUMP;
    if( strcmp(instruction_str, "JZ")==0 ) return RASP_JUMP_IF_ZERO;
    if( strcmp(instruction_str, "JNZ")==0 ) return RASP_JUMP_IF_NOT_ZERO;
    if( strcmp(instruction_str, "JG")==0 ) return RASP_JUMP_IF_GREATER;
    if( strcmp(instruction_str, "JGE")==0 ) return RASP_JUMP_IF_GREATER_OR_EQUALS;
    if( strcmp(instruction_str, "JL")==0 ) return RASP_JUMP_IF_LESS;
    if( strcmp(instruction_str, "JLE")==0 ) return RASP_JUMP_IF_LESS_OR_EQUALS;
    if( strcmp(instruction_str, "PRINT")==0 ) return RASP_PRINT;
    if( strcmp(instruction_str, "HALT")==0 ) return RASP_HALT;
    return -1; // Invalid opcode
}

int get_type_of_operand_from_string( char * operand_str ){
    if( operand_str[0] == '#' ){
        return RASP_OPERAND_FROM_RAW_VALUE;
    } else if( operand_str[0] == '@' ){
        return RASP_OPERAND_FROM_REGISTER_REFERENCE;
    } else {
        return RASP_OPERAND_FROM_REGISTER;
    }
}

void set_operand_from_string( RASP_INSTRUCTION * rasp_instruction, int opcode, char * operand_str ){
    if( is_jump_opcode(opcode) ) {
        rasp_instruction->targetLabel = (char *) safe_malloc(strlen(operand_str) + 1);
        strcpy(rasp_instruction->targetLabel, operand_str);
        return;
    }
    if( operand_str[0] == '#' || operand_str[0] == '@' ){
        rasp_instruction->operand = atoi( &operand_str[1] );
    } else {
        rasp_instruction->operand = atoi( operand_str );
    }
}

unsigned int rasp_instruction_label_hash( char * label ){
    unsigned int hash = 5381;
    int c;
    while ( (c = *label++) ) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}