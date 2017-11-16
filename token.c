
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "token.h"

int make_integer_token(Token *t, char *st, char *filename, int line, int line_index) {
    int base = 10;
    int index = 0;
    int value = 0;

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
            index = 4;
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
            index = 3;
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

    t->type = token_integer;
    t->value.integer = value;
    t->line_number = line;
    t->line_index = line_index;
    t->filename = filename;

    return index;
}

int make_string_token(Token *t, char *st, char *filename, int line, int line_index) {
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
    while (*test++ != '"')
        length += 1;
    /* TODO: factor out escape code handling.
     *
     * while (*test != '"') {
        if (*test == '\\') {
            test++;
            continue;
        }
        length += 1;
    } */

    data = malloc(length + 1);
    strncpy(data, st + 1, length);
    data[length] = '\0';

    t->type = token_string;
    t->value.string = data;
    t->line_number = line;
    t->line_index = line_index;
    t->filename = filename;

    return length + 2;
}

int make_ident_token(Token *t, char *st, char *filename, int line, int line_index) {
    char *data;
    char *test;
    int length = 0;

    test = st;
    while (
        (*test >= 'a' && *test <= 'z') ||
        (*test >= 'A' && *test <= 'Z') ||
        (*test >= '0' && *test <= '9') ||
        (*test == '_')) {

        test++;
        length += 1;
    }

    data = malloc(length + 1);
    strncpy(data, st, length);
    data[length] = '\0';

    t->type = token_ident;
    t->value.string = data;
    t->line_number = line;
    t->line_index = line_index;
    t->filename = filename;

    return length;
}

int make_op_token(Token *t, char *st, char *filename, int line, int line_index) {
    int length;
    int assign = 0;

    /* very TODO: see header for all the ops */

    return 0;
}

int make_other_token(Token *t, char *st, char *filename, int line, int line_index) {
    switch (st[0]) {
    case '{':
        t->type = token_open_brace;
        break;
    case '}':
        t->type = token_close_brace;
        break;
    case '(':
        t->type = token_open_paren;
        break;
    case ')':
        t->type = token_close_paren;
        break;
    default:
        lcc_error(line, line_index, "No acceptable other token at this location");
    }

    t->line_number = line;
    t->line_index = line_index;
    t->filename = filename;
    
    return 1;
}

void print_token(Token *t) {
    switch (t->type) {
    case token_integer:
        printf("Integer(%i) @ %s:%i:%i", t->value.integer, t->filename, t->line_number, t->line_index);
        break;
    case token_string:
        printf("String(%s) @ %s:%i:%i", t->value.string, t->filename, t->line_number, t->line_index);
        break;
    case token_ident:
        printf("Ident(%s) @ %s:%i:%i", t->value.ident, t->filename, t->line_number, t->line_index);
        break;
    case token_open_brace:
        printf("{");
        break;
    case token_close_brace:
        printf("}");
        break;
    case token_open_paren:
        printf("(");
        break;
    case token_close_paren:
        printf(")");
        break;
    default:
        lcc_compiler_error("Unknown token type");
    }
}

int main() {
    Token *int_test = malloc(sizeof(Token));
    Token *int2_test = malloc(sizeof(Token));
    Token *string_test = malloc(sizeof(Token));
    Token *ident_test = malloc(sizeof(Token));
    Token *error_test = malloc(sizeof(Token));
    printf("%i\n", make_integer_token(int_test, "1234", "test.c", 1, 0));
    printf("%i\n", make_integer_token(int2_test, "'\\n'", "test.c", 1, 0));
    printf("%i\n", make_string_token(string_test, "\"Hello World\"", "test.c", 2, 0));
    printf("%i\n", make_ident_token(ident_test, "Hello", "test.c", 2, 0));
    printf("%i\n", make_integer_token(error_test, "1", "test.c", 1, 1));
    error_test->type = 100;

    print_token(int_test);
    printf("\n");
    print_token(int2_test);
    printf("\n");
    print_token(string_test);
    printf("\n");
    print_token(ident_test);
    printf("\n");
    print_token(error_test);
    printf("\n");
}
