#include <stdio.h>

typedef enum {
    RASP_NOOP = 0,
    RASP_LOAD,
    RASP_STORE,
    RASP_ADD,
    RASP_SUB,
    RASP_MUL,
    RASP_DIV,
    RASP_JUMP,
    RASP_JUMP_IF_ZERO,
    RASP_JUMP_IF_NOT_ZERO,
    RASP_JUMP_IF_GREATER,
    RASP_JUMP_IF_GREATER_OR_EQUALS,
    RASP_JUMP_IF_LESS,
    RASP_PRINT,
    RASP_HALT 
} RASP_OPCODE;


void rasp_noop(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_load(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_store(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_add(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_sub(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_mul(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_div(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_zero(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_not_zero(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_greater(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_greater_or_equals(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_less(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_print(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_halt(int typeOfOperand, int operand, RASP_CONTEXT * context);

/*
    RASP_HALT + 1 TO HAVE AN ENTRY FOR EACH OPCODE
*/
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
    rasp_print,
    rasp_halt
};

typedef struct {
    int opcode;
    int typeOfOperand;
    int operand;
    char * label;
} RASP_INSTRUCTION;

typedef struct {
    int should_halt:1;
} RASP_INSTRUCTION_RESULT;

void execute_rasp_instruction( RASP_INSTRUCTION * instruction, RASP_CONTEXT * context, RASP_INSTRUCTION_RESULT * result );

RASP_INSTRUCTION * parse_rasp_instruction( char * line );

unsigned int rasp_instruction_label_hash( char * label );