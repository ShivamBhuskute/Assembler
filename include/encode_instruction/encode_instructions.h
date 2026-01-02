#ifndef ENCODE_INSTR_H
#define ENCODE_INSTR_H

#include <symbol_table/symbol_table.h>
#include <utils/instruction_size.h>

// typedef struct {

// } EncodeINSTR;

void encode_instruction(ParsedLine *pl, SymbolTable *table, int LC);
OpcodeEntry *find_matching_opcode(ParsedLine *pl);

#endif