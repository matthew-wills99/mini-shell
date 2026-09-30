#include <stdio.h>
#include <string.h>

#include "commands.h"

int sh_echo(char** args) {
    int newline = 1;
    size_t i = 1;

    if(args[i] != NULL && strcmp(args[i], "-n") == 0) {
        newline = 0;
        i++;
    }

    for(; args[i] != NULL; i++) {
        fputs(args[i], stdout);
        if(args[i + 1] != NULL) putchar(' ');
    }

    if(newline) putchar('\n');
    fflush(stdout);
    return 1;
}