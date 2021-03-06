
#include <stdio.h>
#include <stdlib.h>

#include "loc.h"
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

void lcc_warning(Location loc, char* message)
{
    /* Get the file at that location */

    printf("%s:%lu:%lu: warning: %s\n", loc.filename, loc.line, loc.index, message);
    return;
}

void lcc_error(Location loc, char* message)
{
    /* Get the file at that location */

    printf("%s:%lu:%lu: error: %s\n", loc.filename, loc.line, loc.index, message);
    exit(1);
}

void lcc_compiler_error_internal(char* file, size_t line, char* message)
{
    /* As above */

    printf("[ ICE ] at %s:%lu\n", file, line);
    printf("%s\n", message);
    exit(1);
}
