#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "utils.h"

bool string_to_int(char* str, int* out) {
    if(str == NULL || out == NULL) return false;

    char *end;
    errno = 0;
    long val = strtol(str, &end, 10);

    if(end == str || *end != '\0' || errno == ERANGE) return false;
    if(val < INT_MIN || val > INT_MAX) return false;

    *out = (int)val;
    return true;
}

bool string_valid_prompt(char* str) {
    if(str == NULL || strlen(str) != sizeof(char)) return false;

    return true;
}