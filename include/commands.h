#ifndef COMMANDS_H
#define COMMANDS_H

#define NUM_BUILTINS (sizeof(builtins) / sizeof(builtins[0]))

int sh_echo(char**);
int sh_cd(char**);
int sh_pwd();
int sh_help();

int c_theme(char**);

typedef struct {
    const char *name;
    int (*fn)(char **args);
    const char *help;
} builtin_t;

static const builtin_t builtins[] = {
    {"echo", sh_echo, "echo a message"},
    {"cd", sh_cd, "change working directory"},
    {"pwd", sh_pwd, "print working directory"},
    {"help", sh_help, "display the help message"},
    {"theme", c_theme, "edit the theme of the shell"},
};

#endif