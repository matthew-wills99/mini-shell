#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "input.h"
#include "shell.h"

#define BUF_SIZE 128

void print_prompt(void) {
    char cwd[PATH_MAX];
    if(!getcwd(cwd, sizeof(cwd))) strcpy(cwd, "?");

    printf("\033[38;5;%dm%s\033[0m%c ", prompt_colour, cwd, prompt_char);
    fflush(stdout);
}

char* read_input(void) {
    char buf[BUF_SIZE];
    char* out = NULL;
    size_t len = 0;

    while(fgets(buf, sizeof(buf), stdin) != NULL) {
        size_t chunk = strlen(buf);

        char* tmp = realloc(out, len + chunk + 1);
        if(tmp == NULL) {
            free(out);
            return NULL;
        }
        out = tmp;

        memcpy(out + len, buf, chunk + 1);
        len += chunk;

        if(out[len - 1] == '\n') {
            out[len - 1] = '\0';
            return out;
        }
    }

    if(out != NULL && len > 0) return out;

    free(out);
    return NULL;
}
