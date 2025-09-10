#ifndef SIZE
#define SIZE
#include <stddef.h>
#include <stdint.h>

#include "parser/parser.h"

typedef enum { OP_NONE, OP_ACC, OP_REG, OP_IMM, OP_MEM, OP_LABEL } OperandType;

typedef enum {
    SIZE_NONE = 0,
    SIZE_BYTE = 1,
    SIZE_WORD = 2,
    SIZE_DWORD = 4,
    SIZE_QEORD = 8
} OperandSize;

typedef struct {
    OperandType type;
    OperandSize size;
    int optional;
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