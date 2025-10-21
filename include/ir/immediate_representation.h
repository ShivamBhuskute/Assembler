#ifndef IR_H
#define IR_H

#include <symbol_table/symbol_table.h>
#include <utils/instruction_size.h>

typedef struct {
    int line_number;
    char* label;
    char* mnemonic;
    char** operands;
    int operand_count;
    int address; // i.e. LC value which im calculating in main file
    int size;
} InstructionIR;

typedef struct {
    InstructionIR* instructions;
    int count;
    int capacity;
} IRList;

void init_IRList(IRList* ir_list);
void free_IRList(IRList* ir_list);
void add_entry_IR(IRList* ir_list, int line_number, const char* label, const char* mnemonic, char** operands, int operand_count, int address);

#endif