#include "utils/instruction_size.h"

#include <stdio.h>
#include <string.h>

OpcodeEntry op_table[] = {
    {"mov", 0xB8, 0, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"mov", 0x89, 0, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_REG, SIZE_DWORD, 0}}, 1},
    {"mov", 0x8B, 0, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_MEM, SIZE_DWORD, 0}}, 1},
    {"mov", 0x89, 0, 2, {{OP_MEM, SIZE_DWORD, 0}, {OP_REG, SIZE_DWORD, 0}}, 1},
    {"mov", 0xC7, 0, 2, {{OP_MEM, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"add", 0x01, 0, 2, {{OP_MEM, SIZE_DWORD, 0}, {OP_REG, SIZE_DWORD, 0}}, 1},
    {"add", 0x03, 0, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_MEM, SIZE_DWORD, 0}}, 1},
    {"add", 0x81, 0, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"add", 0x81, 0, 2, {{OP_MEM, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"add", 0x05, 0, 2, {{OP_ACC, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1}, // add eax, imm32
    {"adc", 0x11, 0, 2, {{OP_MEM, SIZE_DWORD, 0}, {OP_REG, SIZE_DWORD, 0}}, 1},
    {"adc", 0x13, 0, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_MEM, SIZE_DWORD, 0}}, 1},
    {"adc", 0x81, 2, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"adc", 0x81, 2, 2, {{OP_MEM, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"sub", 0x29, 0, 2, {{OP_MEM, SIZE_DWORD, 0}, {OP_REG, SIZE_DWORD, 0}}, 1},
    {"sub", 0x2B, 0, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_MEM, SIZE_DWORD, 0}}, 1},
    {"sub", 0x81, 5, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"sub", 0x81, 5, 2, {{OP_MEM, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"sub", 0x2D, 0, 2, {{OP_ACC, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"sbb", 0x19, 0, 2, {{OP_MEM, SIZE_DWORD, 0}, {OP_REG, SIZE_DWORD, 0}}, 1},
    {"sbb", 0x1B, 0, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_MEM, SIZE_DWORD, 0}}, 1},
    {"sbb", 0x81, 3, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"sbb", 0x81, 3, 2, {{OP_MEM, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"mul", 0xF7, 4, 1, {{OP_MEM, SIZE_DWORD, 0}}, 1}, // mul [reg]
    {"mul", 0xF7, 4, 1, {{OP_REG, SIZE_DWORD, 0}}, 1}, // mul reg
    {"imul", 0xF7, 5, 1, {{OP_MEM, SIZE_DWORD, 0}}, 1},
    {"imul", 0xF7, 5, 1, {{OP_REG, SIZE_DWORD, 0}}, 1},
    {"imul", 0x0FAF, 0, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_REG, SIZE_DWORD, 0}}, 1},
    {"imul", 0x0FAF, 0, 2, {{OP_REG, SIZE_DWORD, 0}, {OP_MEM, SIZE_DWORD, 0}}, 1},
    {"imul", 0x69, 0, 3, {{OP_REG, SIZE_DWORD, 0}, {OP_REG, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"imul", 0x69, 0, 3, {{OP_REG, SIZE_DWORD, 0}, {OP_MEM, SIZE_DWORD, 0}, {OP_IMM, SIZE_DWORD, 0}}, 1},
    {"div", 0xF7, 6, 1, {{OP_MEM, SIZE_DWORD, 0}}, 1},
    {"div", 0xF7, 6, 1, {{OP_REG, SIZE_DWORD, 0}}, 1},
    {"idiv", 0xF7, 7, 1, {{OP_MEM, SIZE_DWORD, 0}}, 1},
    {"idiv", 0xF7, 7, 1, {{OP_REG, SIZE_DWORD, 0}}, 1},
    {"inc", 0xFF, 0, 1, {{OP_MEM, SIZE_DWORD, 0}}, 1},
    {"inc", 0x40, 0, 1, {{OP_REG, SIZE_DWORD, 0}}, 1},
    {"dec", 0xFF, 1, 1, {{OP_MEM, SIZE_DWORD, 0}}, 1},
    {"dec", 0x48, 0, 1, {{OP_REG, SIZE_DWORD, 0}}, 1},
    {"neg", 0xF7, 3, 1, {{OP_MEM, SIZE_DWORD, 0}}, 1},
    {"neg", 0xF7, 3, 1, {{OP_REG, SIZE_DWORD, 0}}, 1},
    {NULL, 0, 0, 0, {{OP_NONE, SIZE_NONE, 0}}, 0}
};


int get_instruction_size(ParsedLine *pl) {
    unsigned int size = 0;
    for (int i = 0; op_table[i].mnemonic != NULL; i++) {
        if (strcmp(pl->mnemonic, op_table[i].mnemonic) == 0) {
            for(int j = 0; j < op_table[i].operand_count; j++) {
                printf("OPERAND: %s\n", op_table[i].mnemonic);
                size += op_table[i].operands[j].size;
            }
            size += op_table[i].size_overhead;
            printf("SIZE: %d", size);
            break;
        }
    }

    return size;
}