#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <utils/instruction_size.h>

#include "encode_instruction/encode_instructions.h"
#include "ir/immediate_representation.h"
#include "parser/parser.h"
#include "symbol_table/symbol_table.h"

bool pass2(IRList* ir_list, SymbolTable* table, const char* output_filename) {
    // Open output file for binary writing
    FILE* output_file = fopen(output_filename, "wb");
    if (!output_file) {
        fprintf(stderr, "Error: Cannot open output file '%s'\n",
                output_filename);
        return false;
    }

    printf("\n=== Pass 2: Code Generation ===\n");

    // Loop through IR
    for (int i = 0; i < ir_list->count; i++) {
        InstructionIR ir = ir_list->instructions[i];

        if (!ir.mnemonic) {
            continue; // im skipping label only lines
        }

        int out_size;
        unsigned char* bytes = encode_instruction(&ir, table, &out_size);

        if (!bytes) {
            fprintf(stderr,
                    "Error: Failed to encode instruction at address %04X\n",
                    ir.address);
            fclose(output_file);
            return false;
        }

        fwrite(bytes, 1, out_size, output_file);

        printf("%04X: ", ir.address);
        for (int j = 0; j < out_size; j++) {
            printf("%02X ", bytes[j]);
        }

        // Print instruction
        printf("  ; %s", ir.mnemonic);
        for (int j = 0; j < ir.operand_count; j++) {
            printf(" %s%s", ir.operands[j],
                   (j < ir.operand_count - 1) ? "," : "");
        }
        printf("\n");

        free(bytes);
    }

    fclose(output_file);
    printf("\nOutput written to '%s'\n", output_filename);
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source.asm>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("Error opening source file");
        return 1;
    }
    const char* output_file = (argc > 2) ? argv[2] : "output.bin";

    SymbolTable table;
    IRList ir_list;
    OpcodeEntry* entry;
    uint16_t matched_opcode = 0;
    init_IRList(&ir_list);
    init_symbol_table(&table);

    char line[256];
    int LC = 0;

    // Pass 1: display code with LC
    while (fgets(line, sizeof(line), f)) {
        ParsedLine* pl = parse_line(line);
        entry = find_matching_opcode(pl);

        // Display current LC address and code
        printf("%04X:    %s", LC, line);

        if (pl->label) {
            add_symbol(&table, pl->label, LC);
            add_entry_IR(&ir_list, pl->label, NULL, NULL, 0, LC, 0, 0);
        }

        if (pl->mnemonic) {
            int size = get_instruction_size(pl);

            if (entry) {
                matched_opcode = entry->base_opcode;
            } else if (!entry) {
                perror("Unknown instruction");
                continue;
            }

            add_entry_IR(&ir_list,
                         pl->label,  // if label exists, store it    once
                         pl->mnemonic, pl->operands, pl->operand_count, LC,
                         size, matched_opcode);

            LC += size;  // updating LC only on an actual instruction
        }

        free_parsed_line(pl);
    }

    fclose(f);

    printf("\n\nSymbol Table:\n");
    for (int i = 0; i < table.count; i++) {
        printf("%s -> %d\n", table.symbols[i].name, table.symbols[i].address);
    }
    
    /*TODO: BELOW print could be bundled in function and used during main
    * assembling for a open operation instead of treating it as a black box*/
   
   printf("\nImmediate Representation:\n");
   for (int i = 0; i < ir_list.count; i++) {
       printf("%04X: ", ir_list.instructions[i].address);
       
       if (ir_list.instructions[i].label)
       printf("%s: ", ir_list.instructions[i].label);
       
       if (ir_list.instructions[i].mnemonic)
       printf("\t%s ", ir_list.instructions[i].mnemonic);
       
       if (ir_list.instructions[i].size)
       printf("\t%d ", ir_list.instructions[i].size);
       
       for (int j = 0; j < ir_list.instructions[i].operand_count; j++) {
           printf("%s", ir_list.instructions[i].operands[j]);
           if (j < ir_list.instructions[i].operand_count - 1) printf(", ");
        }
        
        printf("\n");
    }
    
    if (!pass2(&ir_list, &table, output_file)) {
        fprintf(stderr, "Pass 2 failed\n");
        return 1;
    }
    
    printf("\nAssembly completed successfully!\n");
    
    free_IRList(&ir_list);
    free_symbol_table(&table);
    
    return 0;
}
