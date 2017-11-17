
#include <ctype.h>
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
            index = 1;
            base = 8;
            break;
        default:
            if (isdigit(st[1])) {
                index = 1;
                base = 8;
            }
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

        while (isdigit(st[index])) {
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
    while (isalnum(*test) || (*test == '_')) {
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

void debug_print_token(Token *t) {
    switch (t->type) {
    case token_integer:
        printf("[Integer(%i) @ %s:%i:%i]", t->value.integer, t->filename, t->line_number, t->line_index);
        break;
    case token_string:
        printf("[String(%s) @ %s:%i:%i]", t->value.string, t->filename, t->line_number, t->line_index);
        break;
    case token_ident:
        printf("[Ident(%s) @ %s:%i:%i]", t->value.ident, t->filename, t->line_number, t->line_index);
        break;
    case token_open_brace:
        printf("[ { ]");
        break;
    case token_close_brace:
        printf("[ } ]");
        break;
    case token_open_paren:
        printf("[ ( ]");
        break;
    case token_close_paren:
        printf("[ ) ]");
        break;
    default:
        printf("[Unknown - this is a bug.]");
        /*
         * lcc_compiler_error("Unknown token type");          */
    }
}

#define IF_OP(op, token) if (strncmp(op, program + index, sizeof(op) - 1) == 0) { \
    last->v = malloc(sizeof(Token));    \
    last->v->type = token;              \
    index += sizeof(op) - 1;            \
    line_index += sizeof(op) - 1;       \
}
    

TokenList *tokenize_string(char *program, char *filename) {
    int line_number = 1;
    int line_index = 1;
    int index;
    int program_length = strlen(program);
    int tmp_len;
    TokenList *head = malloc(sizeof(TokenList));
    TokenList *last = head;

    for (index = 0; index < program_length; ) {
        last->v = malloc(sizeof(Token));

        if (program[index] == '\n') {
            line_number += 1;
            line_index = 1;
            index += 1;
            /* Continue so we don't make an empty token */
            continue; 
        } else if (program[index] == ' ') {
            line_index += 1;
            index += 1;
            /* Continue so we don't make an empty token */
            continue;
        } else if (program[index] == '\t') {
            line_index += 4;
            index += 1;
            /* Continue so we don't make an empty token */
            continue;
        } else if (program[index] == '"') {
            tmp_len = make_string_token(last->v, program + index, filename, line_number, line_index);

            index += tmp_len;
            line_index += tmp_len;
        } else if (program[index] == '\'') {
            tmp_len = make_integer_token(last->v, program + index, filename, line_number, line_index);

            index += tmp_len;
            line_index += tmp_len;
        /*  */
        } else if (isdigit(program[index])) {
            tmp_len = make_integer_token(last->v, program + index, filename, line_number, line_index);

            index += tmp_len;
            line_index += tmp_len;
        } else if (isalpha(program[index]) || program[index] == '_') {
            tmp_len = make_ident_token(last->v, program + index, filename, line_number, line_index);

            index += tmp_len;
            line_index += tmp_len;
        }
        
        /* TODO: unhandled
         * if starts with # -> do special things or token it?
         */

        else IF_OP("{", token_open_brace)
        else IF_OP("}", token_close_brace)
        else IF_OP("(", token_open_paren)
        else IF_OP(")", token_close_paren)
        /* TODO: all the other ops */
        
        else {
            lcc_error(line_number, line_index, "Unrecognized token");
        }


        last->next = malloc(sizeof(TokenList));

        last = last->next;

        last->v = NULL;
        last->next = NULL;
    }

    /* TODO: Known issue:
     * The final token will be empty and shows as invalid
     * 
     * this is beause I create the next token and allocate it
     * at the end of the loop, then it checks and finds it
     * needs to return, so it stays uninitialized.
     *
     * I should probably store a previous pointer that
     * will allow be to invalidate that pointer (NULL it)
     */

    return head;
}
/*
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

    debug_print_token(int_test);
    printf("\n");
    debug_print_token(int2_test);
    printf("\n");
    debug_print_token(string_test);
    printf("\n");
    debug_print_token(ident_test);
    printf("\n");
    debug_print_token(error_test);
    printf("\n");

    TokenList *t = tokenize_string("int main \"foo\" 123 '4' '\\n'\nint foo\tbar\n\tfoobar\n\n", "test.c");
    while (t->v != NULL) {
        debug_print_token(t->v);
        printf("\n");
        t = t->next;
    }
}
*/

