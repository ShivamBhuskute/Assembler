#ifndef SIZE
#define SIZE
#include <stddef.h>
#include <stdint.h>

#include "parser/parser.h"

typedef enum { OP_NONE, OP_ACC, OP_REG, OP_IMM, OP_MEM, OP_LABEL } OperandType;

typedef struct {
    OperandType type;
    int size;
} OperandDesc;

typedef struct {
    const char *mnemonic;
    uint16_t base_opcode;
    uint8_t opcode_ext;
    int operand_count;
    OperandDesc operands[3];
    int size_overhead;
} OpcodeEntry;

extern OpcodeEntry op_table[];

int get_instruction_size(ParsedLine *pl);

#endif