#include <stdlib.h>
#include <string.h>

#include "lexer.h"

#define DELIMS " \t\r\n"

char** tokenize(char* in) {
    size_t cap = 8;
    size_t count = 0;
    char** tokens = malloc(cap * sizeof(char *));
    if(tokens == NULL) return NULL;

    char* tok = strtok(in, DELIMS);
    while(tok != NULL) {
        if(count + 1 >= cap) {
            cap *= 2;
            char** tmp = realloc(tokens, cap * sizeof(char *));
            if(tmp == NULL) {
                free(tokens);
                return NULL;
            }
            tokens = tmp;
        }
        tokens[count++] = tok;
        tok = strtok(NULL, DELIMS);
    }

    tokens[count] = NULL;
    return tokens;
}
