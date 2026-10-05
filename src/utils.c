#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

bool string_to_int(char* str, int* out) {
    if(str == NULL || out == NULL) return false;

    char *end;
    long val = strtol(str, &end, 10);

    if(end == str || *end != '\0' || val >= 256) return false;

    *out = (int)val;
    return true;
}

bool string_valid_prompt(char* str) {
    if(str == NULL || strlen(str) != sizeof(char)) return false;

    return true;
}