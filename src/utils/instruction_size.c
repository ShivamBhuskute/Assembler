#include "utils/instruction_size.h"

#include <stdio.h>
#include <string.h>

OpcodeEntry op_table[] = {
    // MOV instructions
    {"mov", 0xB8, 0, 2, {{OP_REG, 0}, {OP_IMM, 4}}, 1},  // MOV reg, imm32, 5
                                                         // bytes total
    {"mov", 0x89, 0, 2, {{OP_MEM, 0}, {OP_REG, 0}}, 1},  // MOV r/m16/32/64,
                                                         // reg, 2-6 bytes
    {"mov",
     0x8B,
     0,
     2,
     {{OP_REG, 0}, {OP_MEM, 0}},
     1},  // MOV reg, r/m16/32/64, 2-6 bytes

    // ADD instructions
    {"add", 0x05, 0, 2, {{OP_ACC, 0}, {OP_IMM, 4}}, 1},  // ADD eax, imm32, 5
                                                         // bytes total
    {"add", 0x01, 0, 2, {{OP_MEM, 0}, {OP_REG, 0}}, 1},  // ADD r/m16/32/64,
                                                         // reg, 2-6 bytes
    {"add",
     0x03,
     0,
     2,
     {{OP_REG, 0}, {OP_MEM, 0}},
     1},  // ADD reg, r/m16/32/64, 2-6 bytes

    // SUB instructions
    {"sub", 0x2D, 0, 2, {{OP_ACC, 0}, {OP_IMM, 4}}, 1},  // SUB eax, imm32, 5
                                                         // bytes total
    {"sub", 0x29, 0, 2, {{OP_MEM, 0}, {OP_REG, 0}}, 1},  // SUB r/m16/32/64,
                                                         // reg, 2-6 bytes
    {"sub",
     0x2B,
     0,
     2,
     {{OP_REG, 0}, {OP_MEM, 0}},
     1},  // SUB reg, r/m16/32/64, 2-6 bytes

    // INC instructions
    {"inc", 0x40, 0, 1, {{OP_REG, 0}}, 0},  // INC reg (opcode 0x40+rd), 1 byte
    {"inc", 0xFF, 0, 1, {{OP_MEM, 0}}, 1},  // INC r/m16/32/64 (Opcode FF /0),
                                            // 2-6 bytes

    // DEC instructions
    {"dec", 0x48, 0, 1, {{OP_REG, 0}}, 0},  // DEC reg (opcode 0x48+rd), 1 byte
    {"dec", 0xFF, 1, 1, {{OP_MEM, 0}}, 1},  // DEC r/m16/32/64 (Opcode FF /1),
                                            // 2-6 bytes

    // JMP instructions
    {"jmp", 0xEB, 0, 1, {{OP_IMM, 1}}, 0},  // JMP rel8 (short jump), 2 bytes
    {"jmp", 0xE9, 0, 1, {{OP_IMM, 4}}, 0},  // JMP rel32 (near jump), 5 bytes

    {NULL, 0, 0, 0, {{OP_NONE, 0}}, 0} // it denptes end
};

int get_instruction_size(ParsedLine* pl) {
    unsigned int size = 0;

    for(int mnemonic = 0; op_table[mnemonic].mnemonic != NULL; mnemonic ++) {
        if(strcmp(pl->mnemonic, op_table[mnemonic].mnemonic) == 0) {
            for(int operandCount = 0; operandCount <= op_table[mnemonic].operand_count; operandCount ++) {
                
            }
        }
    }
}