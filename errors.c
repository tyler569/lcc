
#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

/*
void print_t_line_at_error(char *program, size_t line, size_t ix) {
    size_t index;
    size_t count_nl = 1;

    for (index = 0; ; index++) {
        if (program[index] = '\0') {
            lcc_compiler_error("Line number not in program handling error");
            return;
        }
        if (program[index] = '\n') {
            count_nl += 1;
        }
        if (count_nl == line) {
            break;
        }
    }

    printf("%s", program[index]);

}*/

void lcc_warning(size_t line, size_t ix, char *message) {
    /* Get the file at that location */

    printf("file:%lu:%lu: warning: %s\n", line, ix, message);
    return;
}

void lcc_error(size_t line, size_t ix, char *message) {
    /* Get the file at that location */

    printf("file:%lu:%lu: error: %s\n", line, ix, message);
    exit(1);
}

void lcc_compiler_error_internal(char *file, size_t line, char *message) {
    /* As above */

    printf("[ ICE ] at %s:%lu\n", file, line);
    printf("%s\n", message);
    exit(1);
}

