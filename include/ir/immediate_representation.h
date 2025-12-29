#ifndef IR_H
#define IR_H

#include <symbol_table/symbol_table.h>
#include <utils/instruction_size.h>

typedef struct {
    // int line_number;
    char* label;
    char* mnemonic;
    char** operands;
    int operand_count;
    int address; // i.e. LC value which i'm calculating in main file
    // int size;
} InstructionIR;

typedef struct {
    InstructionIR* instructions;
    int count;
    int capacity;
} IRList;

void init_IRList(IRList* ir_list);
int add_entry_IR(IRList* ir_list, const char* label, const char* mnemonic, char** operands, int operand_count, int address);
void free_IRList(IRList* ir_list);

#endif