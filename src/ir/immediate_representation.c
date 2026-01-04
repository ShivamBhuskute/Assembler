#include "ir/immediate_representation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_IRList(IRList* ir_list) {
    ir_list->count = 0;
    ir_list->capacity = 4;
    ir_list->instructions = malloc(sizeof(InstructionIR) * ir_list->capacity);
    if (!ir_list->instructions) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
}

int add_entry_IR(IRList* ir_list, const char* label, const char* mnemonic,
                 char** operands, int operand_count, int address, int size, uint16_t matched_opcode) {
    if (ir_list->count >= ir_list->capacity) {
        ir_list->capacity *= 2;
        ir_list->instructions = realloc(
            ir_list->instructions, sizeof(InstructionIR) * ir_list->capacity);
    }

    InstructionIR* instr = &ir_list->instructions[ir_list->count];

    instr->label = NULL;
    instr->mnemonic = NULL;
    instr->operands = NULL;
    instr->operand_count = 0;
    instr->address = address;
    instr->size = 0;

    if (label) {
        instr->label = strdup(label);
    }

    if (mnemonic) {
        printf("SAVING MNEMONIC: %s\n", mnemonic);
        instr->mnemonic = strdup(mnemonic);
        instr->size = size;
        instr->matched_opcode = matched_opcode;
    }

    if (operand_count > 0 && operands) {
        instr->operands = malloc(sizeof(char*) * operand_count);
        instr->operand_count = operand_count;
        for (int i = 0; i < operand_count; i++) {
            instr->operands[i] = strdup(operands[i]);
        }
    }

    ir_list->count++;
    return 0;
}

void free_IRList(IRList* ir_list) {
    for (int i = 0; i < ir_list->count; i++) {
        if (ir_list->instructions[i].label)
            free(ir_list->instructions[i].label);

        if (ir_list->instructions[i].mnemonic)
            free(ir_list->instructions[i].mnemonic);

        for (int j = 0; j < ir_list->instructions[i].operand_count; j++) {
            free(ir_list->instructions[i].operands[j]);
        }
        free(ir_list->instructions[i].operands);
    }
    free(ir_list->instructions);
}
