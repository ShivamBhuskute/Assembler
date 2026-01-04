#include "utils/instruction_size.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

OpcodeEntry op_table[] = {
    // MOV instructions - Register variations
    {"mov", 0xB8, 0, 2, {{OP_REG, 0}, {OP_IMM, 2}}, 3},  // MOV AX, imm16
    {"mov", 0xB9, 0, 2, {{OP_REG, 0}, {OP_IMM, 2}}, 3},  // MOV CX, imm16
    {"mov", 0xBA, 0, 2, {{OP_REG, 0}, {OP_IMM, 2}}, 3},  // MOV DX, imm16
    {"mov", 0xBB, 0, 2, {{OP_REG, 0}, {OP_IMM, 2}}, 3},  // MOV BX, imm16
    
    // MOV reg, reg
    {"mov", 0x89, 0, 2, {{OP_REG, 0}, {OP_REG, 0}}, 2},  // MOV reg, reg, 2 bytes
    
    // MOV with memory - Special accumulator (AX) forms
    {"mov", 0xA1, 0, 2, {{OP_ACC, 0}, {OP_MEM, 0}}, 3},  // MOV AX, [addr], 3 bytes
    {"mov", 0xA3, 0, 2, {{OP_MEM, 0}, {OP_ACC, 0}}, 3},  // MOV [addr], AX, 3 bytes
    
    // MOV with memory - General forms
    {"mov", 0x8B, 0, 2, {{OP_REG, 0}, {OP_MEM, 0}}, 3},  // MOV reg, [addr], 3 bytes
    {"mov", 0x89, 0, 2, {{OP_MEM, 0}, {OP_REG, 0}}, 3},  // MOV [addr], reg, 3 bytes

    // ADD instructions
    {"add", 0x05, 0, 2, {{OP_ACC, 0}, {OP_IMM, 2}}, 3},  // ADD AX, imm16, 3 bytes
    {"add", 0x01, 0, 2, {{OP_REG, 0}, {OP_REG, 0}}, 2},  // ADD reg, reg, 2 bytes
    {"add", 0x01, 0, 2, {{OP_MEM, 0}, {OP_REG, 0}}, 3},  // ADD [addr], reg, 3 bytes
    {"add", 0x03, 0, 2, {{OP_REG, 0}, {OP_MEM, 0}}, 3},  // ADD reg, [addr], 3 bytes
    {"add", 0x81, 0, 2, {{OP_REG, 0}, {OP_IMM, 2}}, 4},  // ADD reg, imm16 (opcode + modrm + imm)

    // SUB instructions
    {"sub", 0x2D, 0, 2, {{OP_ACC, 0}, {OP_IMM, 2}}, 3},  // SUB AX, imm16, 3 bytes
    {"sub", 0x29, 0, 2, {{OP_REG, 0}, {OP_REG, 0}}, 2},  // SUB reg, reg, 2 bytes
    {"sub", 0x29, 0, 2, {{OP_MEM, 0}, {OP_REG, 0}}, 3},  // SUB [addr], reg, 3 bytes
    {"sub", 0x2B, 0, 2, {{OP_REG, 0}, {OP_MEM, 0}}, 3},  // SUB reg, [addr], 3 bytes
    {"sub", 0x81, 5, 2, {{OP_REG, 0}, {OP_IMM, 2}}, 4},  // SUB reg, imm16 (opcode + modrm + imm)

    // INC instructions
    {"inc", 0x40, 0, 1, {{OP_REG, 0}, {OP_NONE, 0}}, 1},  // INC reg, 1 byte
    {"inc", 0xFF, 0, 1, {{OP_MEM, 0}, {OP_NONE, 0}}, 3},  // INC [addr], 3 bytes

    // DEC instructions
    {"dec", 0x48, 0, 1, {{OP_REG, 0}, {OP_NONE, 0}}, 1},  // DEC reg, 1 byte
    {"dec", 0xFF, 1, 1, {{OP_MEM, 0}, {OP_NONE, 0}}, 3},  // DEC [addr], 3 bytes

    // JMP instructions
    {"jmp", 0xEB, 0, 1, {{OP_IMM, 1}, {OP_NONE, 0}}, 2},   // JMP rel8 (short), 2 bytes
    {"jmp", 0xE9, 0, 1, {{OP_IMM, 2}, {OP_NONE, 0}}, 3},   // JMP rel16 (near), 3 bytes

    {NULL, 0, 0, 0, {{OP_NONE, 0}, {OP_NONE, 0}}, 0}  // End marker
};

bool is_register(const char* token) {
    if (token == NULL) return false;
    
    return (strcmp(token, "ax") == 0 || 
            strcmp(token, "bx") == 0 || 
            strcmp(token, "cx") == 0 || 
            strcmp(token, "dx") == 0 ||
            strcmp(token, "si") == 0 ||
            strcmp(token, "di") == 0 ||
            strcmp(token, "bp") == 0 ||
            strcmp(token, "sp") == 0);
}

bool is_accumulator(const char* token) {
    if (token == NULL) return false;
    return strcmp(token, "ax") == 0;
}

bool is_memory(const char* token) {
    if (token == NULL) return false;
    
    size_t len = strlen(token);
    if (len >= 2 && token[0] == '[' && token[len-1] == ']') {
        return true;
    }
    
    return false;
}

bool is_immediate(const char* token) {
    if (token == NULL) return false;
    
    if (isdigit(token[0]) || token[0] == '-') {
        return true;
    }
    
    if (token[0] == '0' && (token[1] == 'x' || token[1] == 'X')) {
        return true;
    }
    
    if (!is_register(token) && !is_memory(token)) {
        return true;
    }
    
    return false;
}


bool match_operand_type(const char* operand, OperandType expected_type) {
    if (operand == NULL && expected_type == OP_NONE) {
        return true;
    }
    
    if (operand == NULL) {
        return false;
    }
    
    switch (expected_type) {
        case OP_REG:
            return is_register(operand);
        
        case OP_ACC:
            return is_accumulator(operand);
        
        case OP_IMM:
            return is_immediate(operand);
        
        case OP_MEM:
            return is_memory(operand);
        
        case OP_NONE:
            return (operand == NULL || strlen(operand) == 0);
        
        default:
            return false;
    }
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

int get_instruction_size(ParsedLine* pl) {
    if (pl == NULL || pl->mnemonic == NULL) {
        return -1;
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
            OperandType expected_type = op_table[i].operands[j].type;
            const char* actual_operand = (j < pl->operand_count) ? pl->operands[j] : NULL;
            
            if (!match_operand_type(actual_operand, expected_type)) {
                all_match = false;
                break;
            }
        }
        
        if (all_match) {
            return op_table[i].size_overhead;
        }
    }
    
    fprintf(stderr, "Error: Invalid instruction: '%s'\n", pl->mnemonic);
    return -1;
}