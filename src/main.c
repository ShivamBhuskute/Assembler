#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/instruction_size.h>

#include "ir/immediate_representation.h"
#include "parser/parser.h"
#include "symbol_table/symbol_table.h"

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

    SymbolTable table;
    IRList ir_list;
    init_IRList(&ir_list);
    init_symbol_table(&table);

    char line[256];
    int LC = 0;

    // Pass 1: display code with LC
    while (fgets(line, sizeof(line), f)) {
        ParsedLine* pl = parse_line(line);

        // Display current LC address and code
        printf("%04X:    %s", LC, line);

        if (pl->label) {
            add_symbol(&table, pl->label, LC);
            add_entry_IR(&ir_list, pl->label, NULL, NULL, 0, LC);
        }

        if (pl->mnemonic) {
            int size = get_instruction_size(pl);

            add_entry_IR(&ir_list,
                         pl->label,  // if label exists, store it    once
                         pl->mnemonic, pl->operands, pl->operand_count, LC);

            LC += size;  // updating LC only on an actual instruction
        }

        free_parsed_line(pl);
    }

    fclose(f);

    printf("\n\nSymbol Table:\n");
    for (int i = 0; i < table.count; i++) {
        printf("%s -> %d\n", table.symbols[i].name, table.symbols[i].address);
    }
    free_symbol_table(&table);

    /*TODO: BELOW print could be bundled in function and used during main assembling for a open operation instead of treating it as a black box*/

    printf("\nImmediate Representation:\n");
    for (int i = 0; i < ir_list.count; i++) {
        printf("%04X: ", ir_list.instructions[i].address);

        if (ir_list.instructions[i].label)
            printf("%s: ", ir_list.instructions[i].label);

        if (ir_list.instructions[i].mnemonic)
            printf("\t%s ", ir_list.instructions[i].mnemonic);

        for (int j = 0; j < ir_list.instructions[i].operand_count; j++) {
            printf("%s", ir_list.instructions[i].operands[j]);
            if (j < ir_list.instructions[i].operand_count - 1) printf(", ");
        }

        printf("\n");
    }

    free_IRList(&ir_list);

    return 0;
}
