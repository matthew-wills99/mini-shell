#ifndef COMMANDS_H
#define COMMANDS_H

typedef struct {
    const char *name;
    int (*fn)(char **args);
    const char *help;
} builtin_t;

const builtin_t* find_builtin(const char*);

#endif
