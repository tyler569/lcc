
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "token.h"

typedef struct TokenStr {
    TokenType t;
    char* name;
} TokenStr;

void debug_print_token(Token*);

/* @Note:
 * These are iterated through in order, and therefore
 * any shorter matches must be after longer ones to
 * prevent ambiguities.
 * i.e.:
 * += matching token_plus, token_equal, instead
 * of token_plus_equal.
 */
TokenStr literal_token_names[] = {
    { token_if, "if" },
    { token_for, "for" },
    { token_while, "while" },
    { token_else, "else" },
    { token_switch, "switch" },
    { token_struct, "struct" },
    { token_enum, "enum" },
    { token_union, "union" },
    { token_return, "return" },
    { token_fn, "fn" }, //reserved
    { token_def, "def" }, //reserved
    { token_type, "type" }, //reserved
    { token___FILE__, "__FILE__" },
    { token___LINE__, "__LINE__" },

    { token_bit_left_equal, "<<=" },
    { token_bit_right_equal, ">>=" },
    { token_plus_plus, "++" },
    { token_minus_minus, "--" },
    { token_bit_left, "<<" },
    { token_bit_right, ">>" },
    { token_logic_and, "&&" },
    { token_logic_or, "||" },
    { token_double_equal, "==" },
    { token_not_equal, "!=" },
    { token_less_equal, "<=" },
    { token_greater_equal, ">=" },
    { token_plus_equal, "+=" },
    { token_minus_equal, "-=" },
    { token_star_equal, "*=" },
    { token_slash_equal, "/=" },
    { token_percent_equal, "%=" },
    { token_and_equal, "&=" },
    { token_or_equal, "|=" },
    { token_xor_equal, "^=" },
    { token_access, "->" },
    { token_open_brace, "{" },
    { token_close_brace, "}" },
    { token_open_paren, "(" },
    { token_close_paren, ")" },
    { token_open_bracket, "[" },
    { token_close_bracket, "]" },
    { token_semicolon, ";" },
    { token_equal, "=" },
    { token_plus, "+" },
    { token_minus, "-" },
    { token_star, "*" },
    { token_slash, "/" },
    { token_mod, "%" },
    { token_and, "&" },
    { token_or, "|" },
    { token_xor, "^" },
    { token_not, "!" },
    { token_tilde, "~" },
    { token_less, "<" },
    { token_greater, ">" },
    { token_dot, "." },
    { token_comma, "," },
    { token_question, "?" },
    { token_two_colon, "::" },
    { token_colon, ":" },
    { token_hash, "#" }
};

size_t make_integer_token(Token* t, char* st, char* filename, size_t line,
    size_t line_index)
{
    int base = 10;
    size_t index = 0;

    /* @Bug : doesn't support or handle large integers */
    long int value = 0;

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

        /* This is pretty garbage support for character literals, but it
         * probably works at least a little.
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

        /* TODO: strtol instead of this garbage */

        if (base != 10) {
            lcc_error(line, line_index,
                "I don't support non-base 10 yet, sorry");
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

size_t make_string_token(Token* t, char* st, char* filename, size_t line,
    size_t line_index)
{
    char* data;
    char* test;
    size_t length = 0;

    /* For now, when creating a string token I require the whole thing (or more)
     * to be passed in -- including quotation marks.
     *
     * This may not be a good idea, but ultimately depends on how I end up
     * handling the actual file reading.
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

size_t make_ident_token(Token* t, char* st, char* filename, size_t line,
    size_t line_index)
{
    char* data;
    char* test;
    size_t length = 0;

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

size_t make_other_token(Token* t, char* st, char* filename, size_t line,
    size_t line_index)
{
    int i;
    size_t max = strlen(st);
    size_t cur_len;
    t->type = token_invalid;

    for (i = 0; i < sizeof(literal_token_names) / sizeof(TokenStr); i++) {
        cur_len = strlen(literal_token_names[i].name);

        if (cur_len > max) {
            continue;
        }
        if (strncmp(literal_token_names[i].name, st, cur_len) == 0) {
            t->type = literal_token_names[i].t;
            /* Without this break, it will keep searching everything
             * and ultimately match the *last* match, even if it
             * is shorter
             */
            break;
        }
    }

    if (t->type == token_invalid) {
        return 0;
    }

    t->line_number = line;
    t->line_index = line_index;
    t->filename = filename;

    return cur_len;
}

void debug_print_token(Token* t)
{
    switch (t->type) {
    case token_invalid:
        printf("[Null token - probably uninitialized]");
    case token_integer:
        printf("[Integer(%lu) @ %s:%lu:%lu]", t->value.integer, t->filename,
            t->line_number, t->line_index);
        break;
    case token_string:
        printf("[String(%s) @ %s:%lu:%lu]", t->value.string, t->filename,
            t->line_number, t->line_index);
        break;
    case token_ident:
        printf("[Ident(%s) @ %s:%lu:%lu]", t->value.ident, t->filename,
            t->line_number, t->line_index);
        break;
    default:
        for (int i = 0; i < sizeof(literal_token_names) / sizeof(TokenStr);
             i++) {
            if (t->type == literal_token_names[i].t) {
                printf("[ op: %s ]", literal_token_names[i].name);
                return;
            }
        }
        printf("[Unknown - this is a bug.]");
        /*
         * lcc_compiler_error("Unknown token type");          */
    }
}

#define IF_OP(op, token)                                     \
    if (strncmp(op, program + index, sizeof(op) - 1) == 0) { \
        last->v = malloc(sizeof(Token));                     \
        last->v->type = token;                               \
        index += sizeof(op) - 1;                             \
        line_index += sizeof(op) - 1;                        \
    }

TokenList* tokenize_string(char* program, char* filename)
{
    size_t line_number = 1;
    size_t line_index = 1;
    size_t index;
    size_t program_length = strlen(program);
    size_t tmp_len;
    TokenList* head = malloc(sizeof(TokenList));
    TokenList* last = head;

    for (index = 0; index < program_length;) {
        last->v = malloc(sizeof(Token));

        /* @Bug TODO:
         * Handle EOF everywhere
         */

        if (program[index] == '/' && program[index + 1] == '*') {
            /* Drop for comments */
            while (!(program[index] == '*' && program[index + 1] == '/'))
                index += 1;
            line_index += 1;
            if (program[index] == '\n') {
                line_number += 1;
                line_index = 1;
            }
            index += 2;
        }
        if (program[index] == '/' && program[index + 1] == '/') {
            /* C++ comment */
            while (!(program[index] == '\n'))
                index += 1;
            line_number += 1;
            line_index = 1;
            index += 1;
        }

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
            tmp_len = make_string_token(last->v, program + index, filename,
                line_number, line_index);

            index += tmp_len;
            line_index += tmp_len;
        } else if (program[index] == '\'') {
            tmp_len = make_integer_token(last->v, program + index, filename,
                line_number, line_index);

            index += tmp_len;
            line_index += tmp_len;
            /*  */
        } else if (isdigit(program[index])) {
            tmp_len = make_integer_token(last->v, program + index, filename,
                line_number, line_index);

            lcc_warning(line_number, line_index,
                "Numbers are a warning for testing");

            index += tmp_len;
            line_index += tmp_len;
        } else if (isalpha(program[index]) || program[index] == '_') {
            /* see if it's a keyword first, otherwise it's an ident */
            /* @Speedup: only check tokens that start with alpha | '_' here */
            tmp_len = make_other_token(last->v, program + index, filename,
                line_number, line_index);

            if (tmp_len == 0) {
                tmp_len = make_ident_token(last->v, program + index, filename,
                    line_number, line_index);
            }

            index += tmp_len;
            line_index += tmp_len;
        } else {
            tmp_len = make_other_token(last->v, program + index, filename,
                line_number, line_index);
            /* @Debug token length
            debug_print_token(last->v);
            printf("\ntmp_len: %zu\n\n", tmp_len);
            */

            if (tmp_len == 0) {
                lcc_error(line_number, line_index, "Unrecognized token");
            }

            index += tmp_len;
            line_index += tmp_len;
        }

        last->next = malloc(sizeof(TokenList));

        last = last->next;

        last->v = NULL;
        last->next = NULL;
    }

    /* TODO: @Bug:
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
