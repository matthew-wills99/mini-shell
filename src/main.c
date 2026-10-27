#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

#include "shell.h"
#include "input.h"
#include "lexer.h"
#include "exec.h"

int main(void) {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    bool interactive = isatty(STDIN_FILENO);

    while(1) {
        if(interactive) print_prompt();

        char* in = read_input();
        if(in == NULL) {
            if(interactive) printf("\n");
            break;
        }

        char** args = tokenize(in);
        if(args == NULL) {
            perror("tokenize");
            free(in);
            continue;
        }

        last_status = execute(args);

        free(args);
        free(in);
    }

    shell_cleanup();
    return last_status;
}
