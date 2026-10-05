#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

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