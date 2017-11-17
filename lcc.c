
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "token.h"

int main(int argc, char **argv) {

    FILE *srcfile;
    int i;

    /* Temporary variables: */
    char *buf;
    int len;
    TokenList *tok;

    if (argc < 2) {
        printf("No options specified: exitting\n");
        exit(1);
    }

    for (i=0; i<argc; i++) {
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
        lcc_error(0, 0, "Failed to open file");
        exit(1);
    }

    fseek(srcfile, 0, SEEK_END);
    len = ftell(srcfile);
    rewind(srcfile);

    buf = malloc(len);

    fread(buf, 1, len, srcfile);

    tok = tokenize_string(buf, argv[1]);
    while (tok) {
        debug_print_token(tok->v);
        printf("\n");
        tok = tok->next;
    }
     
    return 0;
}

