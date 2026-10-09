#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

bool string_to_int(char*, int*);
bool string_valid_prompt(char*);

#endif