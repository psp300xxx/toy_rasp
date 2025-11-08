
#ifndef RASP_INSTRUCTION_H
#define RASP_INSTRUCTION_H

#include <stdio.h>

/* Forward declare RASP_CONTEXT to avoid circular includes. The full
   definition is provided in rasp_context.h and is only required by
   implementation files. */
typedef struct RASP_CONTEXT RASP_CONTEXT;

typedef enum {
    RASP_OPERAND_FROM_REGISTER = 0,
    RASP_OPERAND_FROM_RAW_VALUE,
    RASP_OPERAND_FROM_REGISTER_REFERENCE,
    RASP_OPERAND_FROM_LABEL
} RASP_OPERAND_TYPE;

typedef struct {
    int opcode;
    int typeOfOperand;
    int operand;
    char * label;
    char * targetLabel;
    size_t lineNumber;
} RASP_INSTRUCTION;

typedef struct {
    unsigned int should_halt:1;
} RASP_INSTRUCTION_RESULT;

typedef enum {
    RASP_NOOP = 0,
    RASP_LOAD,
    RASP_ETIQUETTE,
    RASP_STORE,
    RASP_ADD,
    RASP_SUB,
    RASP_MUL,
    RASP_MOD,
    RASP_INC,
    RASP_DIV,
    RASP_PRINT_REG,
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
void rasp_noop(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_load(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_etiquette(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_store(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_add(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_sub(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_mul(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_mod(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_inc(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_div(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_print_reg(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_jump(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_jump_if_zero(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_jump_if_not_zero(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_jump_if_greater(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_jump_if_greater_or_equals(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_jump_if_less(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_jump_if_less_or_equals(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_print(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);
void rasp_halt(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);

/* The actual array of function pointers is defined in
   rasp_instruction.c to avoid multiple-definition/linker errors. */
extern void (*RASP_FUNCTIONS[])(RASP_INSTRUCTION * instruction, RASP_CONTEXT * context);

int is_jump_opcode(int opcode);

void print_instruction( RASP_INSTRUCTION * instruction );

void free_rasp_instruction( RASP_INSTRUCTION * instruction );

void execute_rasp_instruction( RASP_INSTRUCTION * instruction, RASP_CONTEXT * context, RASP_INSTRUCTION_RESULT * result );

RASP_INSTRUCTION * parse_rasp_instruction( char * line, int line_number );

int get_opcode_from_string( char * instruction_str );

int is_etiquette(int opcode);

void copy_label_if_non_empty(RASP_INSTRUCTION * rasp_instruction, char * label);

int get_type_of_operand_from_string( char * operand_str );

int get_register_index_from_label( RASP_INSTRUCTION * instruction, RASP_CONTEXT * context );

void fill_rasp_instruction_etiquette(RASP_INSTRUCTION * rasp_instruction, char * operand, char * operand_two);

void set_operand_from_string( RASP_INSTRUCTION * rasp_instruction, char * operand_str );

void increase_program_counter(RASP_CONTEXT * context);

#endif /* RASP_INSTRUCTION_H */