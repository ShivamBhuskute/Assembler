#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table/symbol_table.h"

void init_symbol_table(SymbolTable *table) {
    table->count = 0;
    table->capacity = 4;
    table->symbols = malloc(sizeof(Symbol) * table->capacity);
    if (!table->symbols) {
        fprintf(stderr, "Failed to allocate symbol table\n");
        exit(EXIT_FAILURE);
    }
}

void free_symbol_table(SymbolTable *table) {
    for(int i = 0; i < table->count; i ++) {
        free(table->symbols[i].name);
    }

    free(table->symbols);
}

int add_symbol(SymbolTable *table, const char *name, int address) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return -1;
        }
    }

    if (table->count >= table->capacity) {
        table->capacity *= 2;
        table->symbols = realloc(table->symbols, sizeof(Symbol) * table->capacity);
    }

    table->symbols[table->count].name = strdup(name);
    table->symbols[table->count].address = address;
    table->count++;

    return 0;
}

int find_symbol(SymbolTable *table, const char *name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return table->symbols[i].address;
        }
    }
    return -1;
}