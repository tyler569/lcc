
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

    FILE *srcfile;
    char c;
    int i;
    

    if (argc < 2) {
        printf("No options specified: exitting\n");
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

    srcfile = fopen(argv[1], "r");
    

    while (1) {
        c = fgetc(srcfile);
        
        if (feof(srcfile)) {
            break;
        }

        printf("%c", c);
    }

    return 0;
}

