#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parser/parser.h>

ParsedLine *parse_line(const char *line)
{
    ParsedLine *pl = malloc(sizeof(ParsedLine));
    pl->label = NULL;
    pl->mnemonic = NULL;
    pl->operands = NULL;
    pl->operand_count = 0;

    char *copy = strdup(line);
    char *token = strtok(copy, " \t\n"); // when i did not add space before \t, it gave NULL to labels

    if (!token)
        return pl;

    if (token[strlen(token) - 1] == ':') {
        pl->label = strdup(token);
        pl->label[strlen(pl->label) - 1] = '\0'; // remove ':'
        token = strtok(NULL, " \t\n");
    }

    if (token)
    {
        pl->mnemonic = strdup(token);

        char *operand_str = strtok(NULL, "\n");
        if (operand_str)
        {
            char *op = strtok(operand_str, ",");

            while (op)
            {
                pl->operands = realloc(pl->operands, sizeof(char *) * (pl->operand_count + 1));
                pl->operands[pl->operand_count++] = strdup(op);
                op = strtok(NULL, ",");
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
