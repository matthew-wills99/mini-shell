#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>

#include "commands.h"
#include "shell.h"
#include "utils.h"

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

int sh_cd(char** args) {
    char oldpwd[PATH_MAX];
    char newpwd[PATH_MAX];
    const char *target = args[1] ? args[1] : getenv("HOME");

    if(!getcwd(oldpwd, sizeof(oldpwd))) oldpwd[0] = '\0';

    if(!target || chdir(target) == -1) {
        perror("cd");
        return 1;
    }

    setenv("OLDPWD", oldpwd, 1);

    if(getcwd(newpwd, sizeof(newpwd))) setenv("PWD", newpwd, 1);

    return 0;
}

int sh_pwd() {
    char pwd[PATH_MAX];

    if(getcwd(pwd, sizeof(pwd))) {
        printf("%s\n", pwd);
        fflush(stdout);
        return 0;
    }

    perror("pwd");
    return 1;
}

int sh_help() {

    printf("Help:\n");
    for(int i = 0; i < (int)ARRAY_SIZE(builtins); i++) {
        printf(" %-10s %s\n", builtins[i].name, builtins[i].help);
    }

    fflush(stdout);
    return 0;
}

int c_theme(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "theme: usage: theme [colour] [prompt_char]\n");
        return 1;
    }

    int new_colour = prompt_colour;
    char new_char = prompt_char;
    bool got_colour = false, got_char = false;

    for (int i = 1; args[i] != NULL && i <= 2; i++) {
        int colour;
        if (!got_colour && string_to_int(args[i], &colour)) {
            new_colour = colour;
            got_colour = true;
        } else if (!got_char && string_valid_prompt(args[i])) {
            new_char = args[i][0];
            got_char = true;
        } else {
            fprintf(stderr, "theme: invalid argument '%s'\n", args[i]);
            return 1;
        }
    }

    prompt_colour = new_colour;
    prompt_char = new_char;
    return 0;
}