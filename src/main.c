#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/instruction_size.h>
#include "parser/parser.h"
#include "symbol_table/symbol_table.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source.asm>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Error opening source file");
        return 1;
    }

    SymbolTable table;
    init_symbol_table(&table);

    char line[256];
    int LC = 0;

    // Pass 1: display code with LC
    while (fgets(line, sizeof(line), f)) {
        ParsedLine *pl = parse_line(line);

        // Display current LC address and code
        printf("%04X:    %s", LC, line);

        if (pl->label) {
            add_symbol(&table, pl->label, LC);
        }

        if (pl->mnemonic) {
            int size = get_instruction_size(pl);
            LC += size;
        }

        free_parsed_line(pl);
    }

    fclose(f);

    printf("\n\nSymbol Table:\n");
    for (int i = 0; i < table.count; i++) {
        printf("%s -> %d\n", table.symbols[i].name, table.symbols[i].address);
    }
    free_symbol_table(&table);

    return 0;
}
