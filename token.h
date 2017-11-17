
typedef struct Token {
    enum {
        token_invalid = 0,

        token_integer,
        token_string,
        token_ident,
        token_open_brace,   /* {  */
        token_close_brace,  /* }  */
        token_open_paren,   /* (  */
        token_close_paren,  /* )  */
        token_equal,        /* =  */

        token_plus,         /* +  */
        token_minus,        /* -  */
        token_star,         /* *  */
        token_slash,        /* /  */
        token_mod,          /* %  */

        token_plus_plus,    /* ++ */
        token_minus_minus,  /* -- */

        token_and,          /* &  */
        token_or,           /* |  */
        token_xor,          /* ^  */
        token_not,          /* !  */
        token_tilde,        /* ~  */
        token_bit_left,     /* << */
        token_bit_right,    /* >> */

        token_logic_and,    /* && */
        token_logic_or,     /* || */

        token_double_equal, /* == */
        token_not_equal,    /* != */
        token_less,         /* <  */
        token_greater,      /* >  */
        token_less_equal,   /* <= */
        token_greater_equal,/* >= */

        token_plus_equal,   /* += */
        token_minus_equal,  /* -= */
        token_star_equal,   /* *= */
        token_slash_equal,  /* /= */
        token_percent_equal,    /* %= */
        token_bit_left_equal,   /* <<= */
        token_bit_right_equal,  /* >>= */

        token_and_equal,    /* &= */
        token_or_equal,     /* |= */
        token_xor_equal,    /* ^= */

        token_dot,          /* .  */
        token_access,       /* -> */
        token_comma,        /* ,  */

        token_question,     /* ?  */
        token_colon,        /* :  */
    } type;
    union {
        int integer;
        char *string;
        char *ident;
    } value;

    char *filename;
    int line_number;
    int line_index;
} Token;

typedef struct TokenList {
    Token *v;
    struct TokenList *next;
} TokenList;


/* Functions put the new Token at the provided pointer, which is assumed
 * to be preallocated to sizeof(Token)
 *
 * The number of characters consummed is returned. */

TokenList *tokenize_string(char *program, char *filename);

int make_integer_token(Token *t, char *st, char *fileident, int line, int line_index);
int make_string_token(Token *t, char *st, char *fileident, int line, int line_index);
int make_ident_token(Token *t, char *st, char *fileident, int line, int line_index);

void debug_print_token(Token *t);

