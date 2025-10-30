#include "rasp_instruction.h"
#include "../rasp_executor/rasp_context.h"
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

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
    printf("%d\n", context->accumulator);
    context->program_counter ++;
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
    rasp_instruction->label = label;
    rasp_instruction->opcode = get_opcode_from_string( instruction );
    if( rasp_instruction->opcode == -1 ){
        free( rasp_instruction );
        fprintf( stderr, "Invalid instruction: %s\n", instruction );
        exit(1); // Invalid instruction
    }
    rasp_instruction->typeOfOperand = get_type_of_operand_from_string( operand );
    rasp_instruction->operand = get_operand_from_string( operand );
    return rasp_instruction;
}

int get_opcode_from_string( char * instruction_str ){
    if( strcmp(instruction_str, "NOOP")==0 ) return RASP_NOOP;
    if( strcmp(instruction_str, "LOAD")==0 ) return RASP_LOAD;
    if( strcmp(instruction_str, "STORE")==0 ) return RASP_STORE;
    if( strcmp(instruction_str, "ADD")==0 ) return RASP_ADD;
    if( strcmp(instruction_str, "SUB")==0 ) return RASP_SUB;
    if( strcmp(instruction_str, "MUL")==0 ) return RASP_MUL;
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

int get_operand_from_string( char * operand_str ){
    if( operand_str[0] == '#' || operand_str[0] == '@' ){
        return atoi( &operand_str[1] );
    } else {
        return atoi( operand_str );
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