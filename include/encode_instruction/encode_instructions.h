#ifndef ENCODE_INSTR_H
#define ENCODE_INSTR_H

#include <symbol_table/symbol_table.h>
#include <utils/instruction_size.h>
#include <ir/immediate_representation.h>

// typedef struct {

// } EncodeINSTR;

unsigned char* encode_instruction(InstructionIR* ir, SymbolTable *table, int *out_size);
#endif