#include "encode_instruction/encode_instructions.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/instruction_size.h"

int get_register_code(const char* reg) {
    if (strcmp(reg, "ax") == 0) return 0;
    if (strcmp(reg, "cx") == 0) return 1;
    if (strcmp(reg, "dx") == 0) return 2;
    if (strcmp(reg, "bx") == 0) return 3;
    if (strcmp(reg, "sp") == 0) return 4;
    if (strcmp(reg, "bp") == 0) return 5;
    if (strcmp(reg, "si") == 0) return 6;
    if (strcmp(reg, "di") == 0) return 7;
    return -1;
}

unsigned char create_modrm(int mod, int reg, int rm) {
    return (mod << 6) | (reg << 3) | rm;
}

int parse_immediate(const char* operand, SymbolTable* table) {
    if (!operand) {
        fprintf(stderr, "Error: NULL operand in parse_immediate\n");
        return 0;
    }
    
    // Trim whitespace
    while (*operand && isspace(*operand)) operand++;
    
    int len = strlen(operand);
    
    // this is assembly style
    if (len > 1 && (operand[len-1] == 'h' || operand[len-1] == 'H')) {
        char temp[256];
        strncpy(temp, operand, len-1);
        temp[len-1] = '\0';
        return (int)strtol(temp, NULL, 16);
    }
    
    // this is c-style
    if (len > 2 && operand[0] == '0' && (operand[1] == 'x' || operand[1] == 'X')) {
        return (int)strtol(operand, NULL, 16);
    }
    
    // for decimal
    if (isdigit(operand[0]) || operand[0] == '-') {
        return atoi(operand);
    }
    
    // Otherwise it's a label - look it up
    if (!table) {
        fprintf(stderr, "Error: NULL symbol table\n");
        return 0;
    }
    
    for (int i = 0; i < table->count; i++) {
        if (table->symbols[i].name && strcmp(table->symbols[i].name, operand) == 0) {
            return table->symbols[i].address;
        }
    }
    
    fprintf(stderr, "Error: Undefined label '%s'\n", operand);
    return 0;
}

int extract_address(const char* operand, SymbolTable* table) {
    if (!operand) return 0;

    char temp[256];
    int j = 0;
    for (int i = 0; operand[i]; i++) {
        if (operand[i] != '[' && operand[i] != ']') {
            temp[j++] = operand[i];
        }
    }
    temp[j] = '\0';

    return parse_immediate(temp, table);
}

unsigned char* encode_instruction(InstructionIR* ir, SymbolTable* table,
                                  int* out_size) {
    if (!ir || !ir->mnemonic) {
        *out_size = 0;
        return NULL;
    }

    // Allocate byte array
    unsigned char* bytes = malloc(ir->size);
    if (!bytes) {
        fprintf(stderr, "Memory allocation failed\n");
        *out_size = 0;
        return NULL;
    }

    int byte_index = 0;

    unsigned char opcode = ir->matched_opcode & 0xFF;

    // CASE 1: No operands {would be instructions like HLT, RET, NOP)
    if (ir->operand_count == 0) {
        bytes[byte_index++] = opcode;
        *out_size = byte_index;
        return bytes;
    }

    // CASE 2: Single register operand (INC/DEC reg)
    if (ir->operand_count == 1 && is_register(ir->operands[0])) {
        // For INC (0x40) and DEC (0x48), register is encoded in opcode
        if (opcode == 0x40 || opcode == 0x48) {
            int reg_code = get_register_code(ir->operands[0]);
            if (reg_code < 0) {
                free(bytes);
                *out_size = 0;
                return NULL;
            }
            bytes[0] = opcode + reg_code;
            *out_size = 1;
            return bytes;
        }
    }

    // CASE 3: Single memory operand (INC/DEC [addr])
    if (ir->operand_count == 1 && is_memory(ir->operands[0])) {
        bytes[byte_index++] = opcode;
        int address = extract_address(ir->operands[0], table);
        bytes[byte_index++] = address & 0xFF;         // Low byte
        bytes[byte_index++] = (address >> 8) & 0xFF;  // High byte
        *out_size = byte_index;
        return bytes;
    }

    // CASE 4: Two operands
    if (ir->operand_count == 2) {
        const char* op1 = ir->operands[0];
        const char* op2 = ir->operands[1];

        // SUB-CASE 4a: REG, REG (e.g., mov ax, bx)
        if (is_register(op1) && is_register(op2)) {
            bytes[byte_index++] = opcode;
            int reg1_code = get_register_code(op1);
            int reg2_code = get_register_code(op2);
            unsigned char modrm = create_modrm(0b11, reg1_code, reg2_code);
            bytes[byte_index++] = modrm;
            *out_size = byte_index;
            return bytes;
        }

        // SUB-CASE 4b: REG, IMM (e.g., mov ax, 5)
        if (strcmp(ir->mnemonic, "mov") == 0 && ir->operand_count == 2 &&
            is_register(ir->operands[0]) && is_immediate(ir->operands[1])) {
            int reg_code = get_register_code(ir->operands[0]);
            bytes[0] = 0xB8 + reg_code;  // MOV reg, imm uses 0xB8 + reg
            int immediate = parse_immediate(ir->operands[1], table);
            bytes[1] = immediate & 0xFF;
            bytes[2] = (immediate >> 8) & 0xFF;
            *out_size = 3;
            return bytes;
        }
        if (is_register(op1) && is_immediate(op2)) {
            bytes[byte_index++] = opcode;
            int immediate = parse_immediate(op2, table);
            bytes[byte_index++] = immediate & 0xFF;         // Low byte
            bytes[byte_index++] = (immediate >> 8) & 0xFF;  // High byte
            *out_size = byte_index;
            return bytes;
        }

        // SUB-CASE 4c: REG, MEM (e.g., mov ax, [100])
        if (is_register(op1) && is_memory(op2)) {
            bytes[byte_index++] = opcode;
            int address = extract_address(op2, table);
            bytes[byte_index++] = address & 0xFF;         // Low byte
            bytes[byte_index++] = (address >> 8) & 0xFF;  // High byte
            *out_size = byte_index;
            return bytes;
        }

        // SUB-CASE 4d: MEM, REG (e.g., mov [100], ax)
        if (is_memory(op1) && is_register(op2)) {
            bytes[byte_index++] = opcode;
            int address = extract_address(op1, table);
            bytes[byte_index++] = address & 0xFF;         // Low byte
            bytes[byte_index++] = (address >> 8) & 0xFF;  // High byte
            *out_size = byte_index;
            return bytes;
        }
    }

    // CASE 5: JMP with label (e.g., jmp loop)
    if (strcmp(ir->mnemonic, "jmp") == 0 && ir->operand_count == 1) {
        int target_address = parse_immediate(ir->operands[0], table);

        // Calculate relative offset
        // offset = target - (current_address + instruction_size)
        int offset = target_address - (ir->address + ir->size);

        // Check if short jump is possible (-128 to +127)
        if (offset >= -128 && offset <= 127) {
            bytes[0] = 0xEB;  // Short jump opcode
            bytes[1] = offset & 0xFF;
            *out_size = 2;
            return bytes;
        } else {
            bytes[0] = 0xE9;  // Near jump opcode
            bytes[1] = offset & 0xFF;
            bytes[2] = (offset >> 8) & 0xFF;
            *out_size = 3;
            return bytes;
        }
    }

    // for Unknown pattern
    fprintf(stderr, "Error: Cannot encode instruction '%s'\n", ir->mnemonic);
    free(bytes);
    *out_size = 0;
    return NULL;
}