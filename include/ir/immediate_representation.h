#ifndef IR_H
#define IR_H

#include <symbol_table/symbol_table.h>
#include <utils/instruction_size.h>

typedef struct {
    OperandType type;
    union {
        int reg;
        int imm_16;
        Symbol* sym_ptr;
    } value;
} ParsedOperand;

typedef struct {
    const OpcodeEntry* opcode;
    int instruction_size;
    ParsedOperand operands[2];
} IR;

void init_ir(IR* ir);
void free_ir(IR* ir);

int add_ir(IR* ir, ParsedOperand* operands, const OpcodeEntry* opcode, int address);

#endif