#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"

typedef enum {
    NORMAL,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} LexerState;

#define IS_DELIM(c) ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')

char** tokenize(char* in) {
    size_t cap = 8;
    size_t count = 0;
    char** tokens = malloc(cap * sizeof(char *));
    if (tokens == NULL) return NULL;

    char* r = in;

    while (1) {
        while (IS_DELIM(*r)) r++;
        if (*r == '\0') break;

        if (count + 1 >= cap) {
            cap *= 2;
            char** tmp = realloc(tokens, cap * sizeof(char *));
            if (tmp == NULL) {
                free(tokens);
                return NULL;
            }
            tokens = tmp;
        }

        char* w = r;
        tokens[count++] = w;

        LexerState state = NORMAL;

        while (*r != '\0') {
            if (state == NORMAL) {
                if (*r == '\'') {
                    state = SINGLE_QUOTE;
                }
                else if (*r == '"') {
                    state = DOUBLE_QUOTE;
                }
                else if (IS_DELIM(*r)) {
                    break;
                }
                else {
                    *w++ = *r;
                }
            }
            else if (state == SINGLE_QUOTE) {
                if (*r == '\'') {
                    state = NORMAL;
                }
                else {
                    *w++ = *r;
                }
            }
            else if (state == DOUBLE_QUOTE) {
                if (*r == '"') {
                    state = NORMAL;
                }
                else {
                    *w++ = *r;
                }
            }

            r++;
        }

        if (state != NORMAL) {
            fprintf(stderr, "syntax error: unterminated quote\n");
            free(tokens);
            return NULL;
        }

        if (*r != '\0') r++;

        *w = '\0';
    }

    tokens[count] = NULL;
    return tokens;
}