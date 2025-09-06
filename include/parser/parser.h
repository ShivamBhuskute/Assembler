#ifndef PARSER_H
#define PARSER_H

typedef struct {
    char *label;
    char *mnemonic;
    char **operands;
    int operand_count;
} ParsedLine;

ParsedLine *parse_line(const char *line);

#endif