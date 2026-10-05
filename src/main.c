#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "commands.h"
#include "shell.h"

#define BUF_SIZE 128
#define DELIMS " \t\r\n"

char** tokenize(char*);
char* read_input();
int execute(char**);
int launch(char**);

char prompt_char = '$';
int prompt_colour = 84;

int main() {

    while(1) {
        printf("\033[38;5;%dm%s\e\033[0m%c ", prompt_colour, getenv("PWD"), prompt_char);
        fflush(stdout);

        char* in = read_input();
        if(in == NULL) {
            printf("\n");
            break;
        }

        char** args = tokenize(in);
        if(args == NULL) {
            perror("tokenize");
            free(in);
            continue;
        }

        if(args != NULL) execute(args);
        
        free(args);
        free(in);
    }

    return 0;
}

char* read_input() {
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

int execute(char** args) {
    if (args[0] == NULL) return 1;
    for (size_t i = 0; i < NUM_BUILTINS; i++)
        if (strcmp(args[0], builtins[i].name) == 0)
            return builtins[i].fn(args);
    return launch(args);
}

int launch(char** args) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }
    if (pid == 0) {
        execvp(args[0], args);
        perror(args[0]);
        _exit(127);
    }
    int status;
    waitpid(pid, &status, 0);
    return 1;
}