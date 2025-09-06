#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

typedef struct {
    char *name;
    int address;
} Symbol;

typedef struct {
    Symbol *symbols;
    int count;
    int capacity;
} SymbolTable;

void init_symbol_table(SymbolTable *table);
void free_symbol_table(SymbolTable *table);

int add_symbol(SymbolTable *table, const char *name, int address);
int find_symbol(SymbolTable *table, const char *name);

#endif