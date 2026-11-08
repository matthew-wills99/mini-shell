#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"

#define IS_DELIM(c) ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')

char** tokenize(char* in) {
    size_t cap = 8;
    size_t count = 0;
    char** tokens = malloc(cap * sizeof(char *));
    if(tokens == NULL) return NULL;

    char* r = in;
    while(1) {
        while(IS_DELIM(*r)) r++;
        if(*r == '\0') break;

        if(count + 1 >= cap) {
            cap *= 2;
            char** tmp = realloc(tokens, cap * sizeof(char *));
            if(tmp == NULL) {
                free(tokens);
                return NULL;
            }
            tokens = tmp;
        }

        char* w = r;
        tokens[count++] = w;
        bool in_single = false;

        while(*r != '\0') {
            if(in_single) {
                if(*r == '\'') in_single = false;
                else *w++ = *r;
            } else {
                if(*r == '\'') in_single = true;
                else if(IS_DELIM(*r)) break;
                else *w++ = *r;
            }
            r++;
        }

        if(in_single) {
            fprintf(stderr, "syntax error: unterminated quote\n");
            free(tokens);
            return NULL;
        }

        if(*r != '\0') r++;
        *w = '\0';
    }

    tokens[count] = NULL;
    return tokens;
}
