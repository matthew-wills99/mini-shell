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
    return 0;
}

int sh_cd(char** args) {
    char oldpwd[PATH_MAX];
    char newpwd[PATH_MAX];
    const char *target;
    bool print_dir = false;

    if(args[1] && args[2]) {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }

    if(args[1] == NULL) {
        target = getenv("HOME");
        if(!target) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    } else if(strcmp(args[1], "-") == 0) {
        target = getenv("OLDPWD");
        if(!target) {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return 1;
        }
        print_dir = true;
    } else {
        target = args[1];
    }

    if(!getcwd(oldpwd, sizeof(oldpwd))) oldpwd[0] = '\0';

    if(chdir(target) == -1) {
        perror("cd");
        return 1;
    }

    setenv("OLDPWD", oldpwd, 1);

    if(getcwd(newpwd, sizeof(newpwd))) {
        setenv("PWD", newpwd, 1);
        if(print_dir) {
            printf("%s\n", newpwd);
        }
    }
    
    fflush(stdout);
    return 0;
}

int sh_pwd(char** args) {
    (void)args;

    char pwd[PATH_MAX];

    if(getcwd(pwd, sizeof(pwd))) {
        printf("%s\n", pwd);
        fflush(stdout);
        return 0;
    }

    perror("pwd");
    return 1;
}

int sh_help(char** args) {
    (void)args;

    printf("Help:\n");
    for(int i = 0; i < (int)ARRAY_SIZE(builtins); i++) {
        printf(" %-10s %s\n", builtins[i].name, builtins[i].help);
    }

    fflush(stdout);
    return 0;
}

int sh_theme(char** args) {
    if(args[1] == NULL) {
        fprintf(stderr, "theme: usage: theme [colour] [prompt_char]\n");
        return 1;
    }

    if(args[2] != NULL && args[3] != NULL) {
        fprintf(stderr, "theme: too many arguments\n");
        return 1;
    }

    int new_colour = prompt_colour;
    char new_char = prompt_char;
    bool got_colour = false, got_char = false;

    for(int i = 1; args[i] != NULL && i <= 2; i++) {
        int colour;
        if(!got_colour && string_to_int(args[i], &colour)) {
            if(colour < 0 || colour > 255) {
                fprintf(stderr, "theme: colour out of range (0, 255) '%s'\n", args[i]);
                return 1;
            }
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
    fflush(stdout);
    return 0;
}