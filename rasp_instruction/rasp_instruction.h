
#ifndef RASP_INSTRUCTION_H
#define RASP_INSTRUCTION_H

#include <stdio.h>

/* Forward declare RASP_CONTEXT to avoid circular includes. The full
   definition is provided in rasp_context.h and is only required by
   implementation files. */
typedef struct RASP_CONTEXT RASP_CONTEXT;

typedef enum {
    RASP_OPERAND_FROM_REGISTER = (1 << 0),
    RASP_OPERAND_FROM_RAW_VALUE = (1 << 1),
    RASP_OPERAND_FROM_REGISTER_REFERENCE = (1 << 2)
} RASP_OPERAND_TYPE;

typedef enum {
    RASP_NOOP = 0,
    RASP_LOAD,
    RASP_STORE,
    RASP_ADD,
    RASP_SUB,
    RASP_MUL,
    RASP_MOD,
    RASP_INC,
    RASP_DIV,
    RASP_JUMP,
    RASP_JUMP_IF_ZERO,
    RASP_JUMP_IF_NOT_ZERO,
    RASP_JUMP_IF_GREATER,
    RASP_JUMP_IF_GREATER_OR_EQUALS,
    RASP_JUMP_IF_LESS,
    RASP_JUMP_IF_LESS_OR_EQUALS,
    RASP_PRINT,
    RASP_HALT
} RASP_OPCODE;

/* Function prototypes for instruction implementations (defined in .c).
   These only reference RASP_CONTEXT via pointer so the forward
   declaration above is sufficient here. */
void rasp_noop(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_load(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_store(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_add(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_sub(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_mul(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_mod(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_inc(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_div(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_zero(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_not_zero(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_greater(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_greater_or_equals(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_less(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_jump_if_less_or_equals(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_print(int typeOfOperand, int operand, RASP_CONTEXT * context);
void rasp_halt(int typeOfOperand, int operand, RASP_CONTEXT * context);

/* The actual array of function pointers is defined in
   rasp_instruction.c to avoid multiple-definition/linker errors. */
extern void (*RASP_FUNCTIONS[])(int typeOfOperand, int operand, RASP_CONTEXT * context);

int is_jump_opcode(int opcode);

typedef struct {
    int opcode;
    int typeOfOperand;
    int operand;
    char * label;
    char * targetLabel;
} RASP_INSTRUCTION;

typedef struct {
    unsigned int should_halt:1;
} RASP_INSTRUCTION_RESULT;

void execute_rasp_instruction( RASP_INSTRUCTION * instruction, RASP_CONTEXT * context, RASP_INSTRUCTION_RESULT * result );

RASP_INSTRUCTION * parse_rasp_instruction( char * line );

int get_opcode_from_string( char * instruction_str );

void copy_label_if_non_empty(RASP_INSTRUCTION * rasp_instruction, char * label);

int get_type_of_operand_from_string( char * operand_str );

void set_operand_from_string( RASP_INSTRUCTION * rasp_instruction, int opcode, char * operand_str );

unsigned int rasp_instruction_label_hash( char * label );

void increase_program_counter(RASP_CONTEXT * context);

#endif /* RASP_INSTRUCTION_H */