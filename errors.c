
#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

/*
void print_line_at_error(char *program, int line, int ix) {
    int index;
    int count_nl = 1;

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

void lcc_warning(int line, int ix, char *message) {
    /* Get the file at that location */

    printf("WARNING: %s at %i:%i\n", message, line, ix);
    return;
}

void lcc_error(int line, int ix, char *message) {
    /* Get the file at that location */

    printf("ERROR: %s at %i:%i\n", message, line, ix);
    exit(1);
}

void lcc_compiler_error_internal(char *file, int line, char *message) {
    /* As above */

    printf("INTERNAL COMPILER ERROR at %s:%i\n", file, line);
    printf("%s\n", message);
    exit(1);
}

