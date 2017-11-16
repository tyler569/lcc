
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"

typedef struct Token {
    enum {
        token_integer,
        token_string,
        token_name,
    } type;
    union {
        int integer;
        char *string;
        char *name;
    } value;

    char *filename;
    int line_number;
    int line_index;
} Token;

Token *make_integer_token(char *st, char *filename, int line, int line_index) {
    int base = 10;
    int index = 0;
    int value = 0;
    Token *result;

    if (st[0] == '0') {
        switch (st[1]) {
        case 'x':
            base = 16;
            index = 2;
            break;
        case 'b':
            base = 2;
            index = 2;
            break;
        case 'o':
        default:
            index = 1;
            base = 8;
        }
    }

    if (st[0] == '\'') {

        /* This is pretty garbage support for character literals, but it probably works
         * at least a little.
         *
         * I will try to clean this up later.
         *
         * TODO handle \x12, \u1234, \U12345678 and other char codes */

        if (st[1] == '\\') {
            if (st[3] != '\'') {
                lcc_error(line, line_index, "expected closing \"'\".");
            }
            switch (st[2]) {
            case 'n':
                value = '\n';
                break;
            default:
                lcc_compiler_error("I don't support that character code yet");
            }
        } else {
            if (st[2] != '\'') {
                lcc_error(line, line_index, "expected closing \"'\".");
            }
            value = st[1];
        }
    } else {

        if (base != 10) {
            lcc_error(line, line_index, "I don't support non-base 10 yet, sorry");
        }

        while (st[index] >= '0' && st[index] <= '9') {
            value *= base;
            value += st[index] - '0';
            index += 1;
        }

    }

    result = malloc(sizeof(Token));

    result->type = token_integer;
    result->value.integer = value;
    result->line_number = line;
    result->line_index = line_index;
    result->filename = filename;

    return result;
}

Token *make_string_token(char *st, char *filename, int line, int line_index) {
    Token *result;
    char *data;
    char *test;
    int length = 0;

    /* For now, when creating a string token I require the whole thing (or more) to be
     * passed in -- including quotation marks.
     *
     * This may not be a good idea, but ultimately depends on how I end up handling the
     * actual file reading.
     *
     * TODO I am leaving off escape handling for now. */

    if (st[0] != '"') {
        lcc_compiler_error("make_string_token called without being string");
    }

    test = st + 1;
    while (*test++ != '"');
    /* TODO: factor out escape code handling.
     *
     * while (*test != '"') {
        if (*test == '\\') {
            test++;
            continue;
        }
        length += 1;
    } */

    data = malloc(length) + 1;
    strncpy(data, st + 1, length);

    result = malloc(sizeof(Token));    

    result->type = token_string;
    result->value.string = data;
    result->line_number = line;
    result->line_index = line_index;
    result->filename = filename;

    return result;
}

Token *make_name_token(char *st, char *filename, int line, int line_index) {
    Token *result;
    char *data;
    char *test;
    int length = 0;

    test = st + 1;
    while (*test++ )
        length += 1;

    data = malloc(length);
    strncpy(data, st + 1, length);

    result = malloc(sizeof(Token));    

    result->type = token_string;
    result->value.string = data;
    result->line_number = line;
    result->line_index = line_index;
    result->filename = filename;

    return result;
}
void print_token(Token *t) {
    switch (t->type) {
    case token_integer:
        printf("Integer(%i) @ %s:%i:%i", t->value.integer, t->filename, t->line_number, t->line_index);
        break;
    case token_string:
        printf("String(%s) @ %s:%i:%i", t->value.string, t->filename, t->line_number, t->line_index);
        break;
    default:
        lcc_compiler_error("Unknown token type");
    }
}

int main() {
    Token *int_test = make_integer_token("1234", "test.c", 1, 0);
    Token *string_test = make_string_token("\"Hello World\"", "test.c", 2, 0);
    Token *error_test = make_integer_token("1", "test.c", 1, 1);
    error_test->type = 100;

    print_token(int_test);
    printf("\n");
    print_token(string_test);
    printf("\n");
    print_token(error_test);
    printf("\n");
}
