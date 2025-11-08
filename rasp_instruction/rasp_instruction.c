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
void (*RASP_FUNCTIONS[RASP_HALT + 1])(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context) = {
    rasp_noop,
    rasp_load,
    rasp_etiquette,
    rasp_store,
    rasp_add,
    rasp_sub,
    rasp_mul,
    rasp_mod,
    rasp_inc,
    rasp_div,
    rasp_print_reg,
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

void rasp_jump(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    context->program_counter = instruction->operand;
}

void rasp_load(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    switch( instruction->typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            context->accumulator = context->registers[ instruction->operand ];
            break;
        case RASP_OPERAND_FROM_RAW_VALUE:
            context->accumulator = instruction->operand;
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            context->accumulator = context->registers[ context->registers[ instruction->operand ] ];
            break;
        case RASP_OPERAND_FROM_LABEL:
            int operand = get_integerstrkey( instruction->targetLabel, context->etiquettes);
            context->accumulator = operand;
    }
    increase_program_counter( context );
}

void rasp_print_reg(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int typeOfOperand = instruction->typeOfOperand;
    switch ( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            printf("%d\n", context->registers[ instruction->operand ]);
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            printf("%d\n", context->registers[ context->registers[ instruction->operand ] ] );
            break;
        case RASP_OPERAND_FROM_LABEL:
            printf("%d\n", context->registers[ get_integerstrkey( instruction->targetLabel, context->etiquettes ) ] );
            increase_program_counter( context );
    }
    increase_program_counter( context );
}

void rasp_store(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int typeOfOperand = instruction->typeOfOperand;
    switch ( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            context->registers[ instruction->operand ] = context->accumulator;
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            context->registers[ context->registers[ instruction->operand ] ] = context->accumulator;
            break;
        case RASP_OPERAND_FROM_LABEL:
            {
                int register_index = get_integerstrkey( instruction->targetLabel, context->etiquettes );
                context->registers[ register_index ] = context->accumulator;
            }
    }
    increase_program_counter( context );
}

void rasp_etiquette(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    put_integerstrkey( instruction->label, instruction->operand, context->etiquettes );
    increase_program_counter( context );
}

void rasp_add(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    int typeOfOperand = instruction->typeOfOperand;
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
        case RASP_OPERAND_FROM_LABEL:
            {
                int source_register = get_integerstrkey( instruction->targetLabel, context->etiquettes );
                context->accumulator += context->registers[ source_register ];
            }
    }
    increase_program_counter( context );
}

void rasp_sub(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    int typeOfOperand = instruction->typeOfOperand;
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
        case RASP_OPERAND_FROM_LABEL:
            {
                int source_register = get_integerstrkey( instruction->targetLabel, context->etiquettes );
                context->accumulator -= context->registers[ source_register ];
            }
    }
    increase_program_counter( context );
}

void rasp_mod(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    int typeOfOperand = instruction->typeOfOperand;
    int denominator;
    switch( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            denominator = context->registers[ operand ];
            break;
        case RASP_OPERAND_FROM_RAW_VALUE:
            denominator = operand;
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            denominator = context->registers[ context->registers[ operand ] ];
            break;
        case RASP_OPERAND_FROM_LABEL:
            denominator = context-> registers[ get_integerstrkey( instruction->targetLabel, context->etiquettes ) ];
            break;  
    }
    if( denominator == 0 ){
        fprintf( stderr, "Error: Modulo by zero at instruction on line %zu\n", instruction->lineNumber );
        context->last_instruction_result->should_halt = 1;
        return;
    }
    context->accumulator %= denominator;
    increase_program_counter( context );
}

void rasp_inc(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    int typeOfOperand = instruction->typeOfOperand;
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

void rasp_mul(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    int typeOfOperand = instruction->typeOfOperand;
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
        case RASP_OPERAND_FROM_LABEL:
            {
                int source_register = get_integerstrkey( instruction->targetLabel, context->etiquettes );
                context->accumulator *= context->registers[ source_register ];
            }
    }
    increase_program_counter( context );
}

void rasp_div(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    int typeOfOperand = instruction->typeOfOperand;
    int denominator;
    switch( typeOfOperand ){
        case RASP_OPERAND_FROM_REGISTER:
            denominator = context->registers[ operand ];
            break;
        case RASP_OPERAND_FROM_RAW_VALUE:
            denominator = operand;
            break;
        case RASP_OPERAND_FROM_REGISTER_REFERENCE:
            denominator = context->registers[ context->registers[ operand ] ];
            break;
        case RASP_OPERAND_FROM_LABEL:
            denominator = context-> registers[ get_integerstrkey( instruction->targetLabel, context->etiquettes ) ];
            break;
    }
    if( denominator == 0 ){
        fprintf( stderr, "Error: Division by zero at instruction on line %zu\n", instruction->lineNumber );
        context->last_instruction_result->should_halt = 1;
        return;
    }
    context->accumulator /= denominator;
    increase_program_counter( context );
}

void rasp_noop(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    increase_program_counter( context );
}   

void rasp_jump_if_zero(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    if( context->accumulator == 0 ){
        context->program_counter = operand;
        return;
    }
    increase_program_counter( context );
}

void rasp_halt(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    context->last_instruction_result->should_halt = 1;
    printf("HALT encountered. Stopping execution.\n");
}

void rasp_jump_if_greater(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    if( context->accumulator > 0 ){
        context->program_counter = operand;
        return;
    }
    increase_program_counter( context );
}

void rasp_jump_if_not_zero(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    if( context->accumulator != 0 ){
        context->program_counter = operand;
        return;
    }
    increase_program_counter( context );
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

void rasp_jump_if_greater_or_equals(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    if( context->accumulator >= 0 ){
        context->program_counter = operand;
        return;
    }
    increase_program_counter( context );
}

void execute_rasp_instruction( RASP_INSTRUCTION * instruction, RASP_CONTEXT * context, RASP_INSTRUCTION_RESULT * result ){
    int opcode = instruction->opcode;
    RASP_FUNCTIONS[ opcode ]( instruction, context ); 
    result->should_halt = context->last_instruction_result->should_halt;
}

void rasp_jump_if_less(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    if( context->accumulator < 0 ){
        context->program_counter = operand;
        return;
    }
    increase_program_counter( context );
}

void rasp_jump_if_less_or_equals(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    int operand = instruction->operand;
    if( context->accumulator <= 0 ){
        context->program_counter = operand;
        return;
    }
    increase_program_counter( context );
}

void rasp_print(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context){
    printf("%d\n", context->accumulator);
    increase_program_counter( context );
}

int is_etiquette(int opcode){
    return ( opcode == RASP_ETIQUETTE );
}

void free_rasp_instruction( RASP_INSTRUCTION * instruction ){
    if( instruction == NULL ) return;
    if( instruction->label != NULL ){
        free( instruction->label );
    }
    if( instruction->targetLabel != NULL ){
        free( instruction->targetLabel );
    }
    free( instruction );
}


RASP_INSTRUCTION * parse_rasp_instruction( char * line, int line_number ){
    char label[64] = "";
    char instruction[64] = "";
    char operand[64] = "";
    char operand_two[64] = "";

    // Rimuove spazi iniziali
    while (isspace((unsigned char)*line)) line++;

    if( line[0]== '#' ) {
        RASP_INSTRUCTION * rasp_instruction = (RASP_INSTRUCTION *) safe_malloc( sizeof(RASP_INSTRUCTION) );
        rasp_instruction->opcode = RASP_NOOP;
        return rasp_instruction;
    }
    else if (strchr(line, ':')) {
        // LABEL: INSTRUCTION OPERAND
        sscanf(line, "%63[^:]: %63s %63s", label, instruction, operand);
    } else {
        // INSTRUCTION OPERAND
        sscanf(line, "%63s %63s %63s", instruction, operand, operand_two);
    }
    RASP_INSTRUCTION * rasp_instruction = (RASP_INSTRUCTION *) safe_malloc( sizeof(RASP_INSTRUCTION) );
    rasp_instruction->lineNumber = line_number;
    copy_label_if_non_empty(rasp_instruction, label);
    rasp_instruction->opcode = get_opcode_from_string( instruction );
    if( rasp_instruction->opcode == -1 ){
        free_rasp_instruction( rasp_instruction );
        fprintf( stderr, "Invalid instruction: %s\n", instruction );
        exit(1); // Invalid instruction
    }
    if( is_etiquette( rasp_instruction->opcode ) ){
        rasp_instruction->typeOfOperand = RASP_OPERAND_FROM_RAW_VALUE;
        set_operand_from_string( rasp_instruction, rasp_instruction->opcode, operand );
        if( strlen( operand_two ) == 0  ){
            rasp_instruction->label = NULL;
            fprintf( stderr, "Error: etiquette instruction missing label\n" );
            return NULL;
        }
        rasp_instruction->label = safe_malloc( strlen( operand_two ) + 1 );
        strcpy( rasp_instruction->label, operand_two );
        return rasp_instruction;
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
    if( strcmp(instruction_str, "ETIQ")==0 ) return RASP_ETIQUETTE;
    if( strcmp(instruction_str, "PRINT_REG")==0 ) return RASP_PRINT_REG;
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
    } else if ( operand_str[0] == '$' ){
        return RASP_OPERAND_FROM_LABEL;
    }
    else {
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
    }
    else if( operand_str[0] == '$' ){
        int len = strlen(operand_str);
        rasp_instruction->targetLabel = safe_malloc(len);
        rasp_instruction->targetLabel[len] = 0;
        memcpy(rasp_instruction->targetLabel, operand_str + 1, len-1); // No operand provided
    }
    else {
        rasp_instruction->operand = atoi( operand_str );
    }
}