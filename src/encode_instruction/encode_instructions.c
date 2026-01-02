#include "encode_instruction/encode_instructions.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int get_register_codes(char* reg) {
    if (register_name == "ax") return 0;
    if (register_name == "cx") return 1;
    if (register_name == "dx") return 2;
    if (register_name == "bx") return 3;
    if (register_name == "sp") return 4;
    if (register_name == "bp") return 5;
    if (register_name == "si") return 6;
    if (register_name == "di") return 7;
    return -1;
}

unsigned char* create_modrmbyte(int mod, int reg, int rm) {
    return (mod << 6) | (reg << 3) | rm;
}

OpcodeEntry* find_matching_opcode(ParsedLine* pl) {
    if (pl == NULL || pl->mnemonic == NULL) {
        return NULL;
    }

    for (int i = 0; op_table[i].mnemonic != NULL; i++) {
        if (strcmp(pl->mnemonic, op_table[i].mnemonic) != 0) {
            continue;
        }

        if (pl->operand_count != op_table[i].operand_count) {
            continue;
        }

        bool all_match = true;
        for (int j = 0; j < op_table[i].operand_count; j++) {
            if (!match_operand_type(pl->operands[j],
                                    op_table[i].operands[j].type)) {
                all_match = false;
                break;
            }
        }

        if (all_match) {
            return &op_table[i];
        }
    }

    return NULL;
}

void encode_instruction(ParsedLine* pl, SymbolTable* table, int LC) {
    OpcodeEntry* opcode_entry = find_matching_opcode(pl);

    if (opcode_entry == NULL) {
        fprintf(stderr, "Unknown instruction\n");
        return NULL;
    }

    unsigned char* bytes = malloc(opcode_entry->size_overhead);
    int byte_index = 0;

    bytes[byte_index++] = opcode_entry->base_opcode;
}