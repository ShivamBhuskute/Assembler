#include "ir/immediate_representation.h"

void init_IRList(IRList* ir_list) {
    IRList* ir_list = malloc(sizeof(IRList));
    ir_list->count = 0;
    ir_list->capacity = 4;
    ir_list->instructions = malloc(sizeof(InstructionIR) * ir_list->capacity);
    if (!ir_list->instructions) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
}

int add_entry_IR(IRList* ir_list, int line_number, const char* label,
                 const char* mnemonic, char** opernds, int operand_count,
                 int address) {
    ;
}

void free_IRList(IRList* ir_list) {
    for (int i = 0; i < ir_list->count; i++) {
        free(ir_list->instructions[i].label);
        free(ir_list->instructions[i].mnemonic);
        for (int j = 0; j < ir_list->instructions[i].operand_count; j++) {
            free(ir_list->instructions[i].operands[j]);
        }
        free(ir_list->instructions[i].operands);
    }
    free(ir_list->instructions);
    free(ir_list);
}