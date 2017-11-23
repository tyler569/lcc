
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "token.h"
#include "vector.h"

int main(int argc, char** argv)
{

    FILE* srcfile;
    int i;

    /* Temporary variables: */
    char* buf;
    size_t len;
    Vector *tokens;

    if (argc < 2) {
        printf("No options specified: exitting\n");
        exit(1);
    }

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            printf("Help.");
            return 0;
        } /* other options */
        else {
            /* files */
        }
    }

    /* TODO: ony actually open the file */

    srcfile = fopen(argv[1], "r");

    if (srcfile == NULL) {
        printf("Failed to open file\n");
        exit(1);
    }

    fseek(srcfile, 0, SEEK_END);
    len = ftell(srcfile);
    rewind(srcfile);

    buf = malloc(len + 1);
    buf[len] = 0;

    fread(buf, 1, len, srcfile);

    tokens = tokenize_string(buf, argv[1]);

    printf("\nDebug printing tokens:\n\n");
    for (i=0; i<tokens->len; i++) {
        debug_print_token(vec_get(tokens, i));
        printf("\n");
    }

    return 0;
}
