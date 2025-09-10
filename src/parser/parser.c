#include <parser/parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ParsedLine *parse_line(const char *line) {
    ParsedLine *pl = malloc(sizeof(ParsedLine));
    pl->label = NULL;
    pl->mnemonic = NULL;
    pl->operands = NULL;
    pl->comment = NULL;
    pl->operand_count = 0;

    char *copy = strdup(line);
    char *token = strtok(
        copy,
        " \t\n");  // when i did not add space before \t, it gave NULL to labels

    if (!token) return pl;

    if (token[strlen(token) - 1] == ':') {
        // printf("TOKEN_LABEL%s\n", token);
        pl->label = strdup(token);
        pl->label[strlen(pl->label) - 1] = '\0';  // removes ':'
        token = strtok(NULL, " \t\n");
    }

    if (token) {
        printf("TOKEN: %s\n", token);
        pl->mnemonic = strdup(token);
        // printf("MNEMONIC: %s\n", pl->mnemonic);

        char *operand_str = strtok(NULL, "\n");
        printf("OP %s\n", operand_str);
        if (operand_str) {
            char *op = strtok(operand_str, ",");

            printf("OPINWHILE: %s\n", op);
            while (op) {
                pl->operands = realloc(
                    pl->operands, sizeof(char *) * (pl->operand_count + 1));
                pl->operands[pl->operand_count++] = strdup(op);
                op = strtok(NULL, ",");
                printf("OPINWHILE: %s\n", op);
            }
        }
    }

    free(copy);
    return pl;
}

void free_parsed_line(ParsedLine *pl) {
    if (!pl) return;
    free(pl->label);
    free(pl->mnemonic);
    for (int i = 0; i < pl->operand_count; i++) {
        free(pl->operands[i]);
    }
    free(pl->operands);
    free(pl);
}
