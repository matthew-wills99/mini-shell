#ifndef COMMANDS_H
#define COMMANDS_H

#define NUM_BUILTINS (sizeof(builtins) / sizeof(builtins[0]))

int sh_echo(char**);

typedef struct {
    const char *name;
    int (*fn)(char **args);
    const char *help;
} builtin_t;

static const builtin_t builtins[] = {
    {"echo", sh_echo, "echo a message"},
};

#endif