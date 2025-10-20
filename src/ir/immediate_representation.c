#include "ir/immediate_representation.h"

void init_ir(IR* ir) {
    ir->opcode = 0;
    ir->instruction_size = 0;
    ir->operands = malloc(sizeof(ParsedOperand) * 2);
    if (!ir->operands) {
        fprintf(stderr, "Failed to allocate IR opetrands\n");
        exit(EXIT_FAILURE);
    }
}

void free_ir(IR* ir) {
    free(ir->operands); // i think this is incorrect, prolly have to free individually i think
}

void add_ir(IR* ir, ParsedOperand* operands, const OpcodeEntry* opcode, int address) {
    ;
    return 0;
}

