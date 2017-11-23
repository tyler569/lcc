
#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

#include "vector.h"
#include "loc.h"

typedef enum TokenType {
    token_invalid = 0,

    token_integer,
    token_string,
    token_ident,
    token_open_brace, /* {  */
    token_close_brace, /* }  */
    token_open_paren, /* (  */
    token_close_paren, /* )  */
    token_open_bracket, /* [  */
    token_close_bracket, /* ]  */
    token_semicolon, /* ;  */
    token_equal, /* =  */

    token_plus, /* +  */
    token_minus, /* -  */
    token_star, /* *  */
    token_slash, /* /  */
    token_mod, /* %  */

    token_plus_plus, /* ++ */
    token_minus_minus, /* -- */

    token_and, /* &  */
    token_or, /* |  */
    token_xor, /* ^  */
    token_not, /* !  */
    token_tilde, /* ~  */
    token_bit_left, /* << */
    token_bit_right, /* >> */

    token_logic_and, /* && */
    token_logic_or, /* || */

    token_double_equal, /* == */
    token_not_equal, /* != */
    token_less, /* <  */
    token_greater, /* >  */
    token_less_equal, /* <= */
    token_greater_equal, /* >= */

    token_plus_equal, /* += */
    token_minus_equal, /* -= */
    token_star_equal, /* *= */
    token_slash_equal, /* /= */
    token_percent_equal, /* %= */
    token_bit_left_equal, /* <<= */
    token_bit_right_equal, /* >>= */

    token_and_equal, /* &= */
    token_or_equal, /* |= */
    token_xor_equal, /* ^= */

    token_dot, /* .  */
    token_access, /* -> */
    token_comma, /* ,  */
    token_two_colon, /* :: */

    token_question, /* ?  */
    token_colon, /* :  */
    token_hash, /* #  */

    token_if, /* if */
    token_for, /* for */
    token_while, /* while */
    token_else, /* else */
    token_switch, /* switch */
    token_struct, /* struct */
    token_enum, /* enum */
    token_union, /* union */
    token_return, /* return */
    token_fn,
    /* fn */ //reserved
    token_def,
    /* def */ //reserved
    token_type,
    /* type */ //reserved
    token___FILE__, /* __FILE__ */
    token___LINE__, /* __LINE__ */

    token_invalid_end
} TokenType;

typedef struct Token {
    TokenType type;
    union {
        long int integer;
        char* string;
        char* ident;
    } value;

    Location loc;
} Token;

/*
typedef struct TokenList {
    Token* v;
    struct TokenList* next;
} TokenList;
*/

/* Functions put the new Token at the provided pointer, which is assumed
 * to be preallocated to sizeof(Token)
 *
 * The number of characters consummed is returned. */

Vector* tokenize_string(char* program, char* filename);

size_t make_integer_token(Token* t, char* st, Location loc);
size_t make_string_token(Token* t, char* st, Location loc);
size_t make_ident_token(Token* t, char* st, Location loc);

void debug_print_token(Token* t);

#endif
