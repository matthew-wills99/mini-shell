#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 128

char* read_input();

int main() {

    while(1) {
        printf("> ");
        fflush(stdout);

        char *in = read_input();
        if(in == NULL) {
            printf("\n");
            break;
        }

        printf("%s\n", in);
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