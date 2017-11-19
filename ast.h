
#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef struct AstExp {
    enum {
        ast_integer, /* 'c' literals are integers */
        /*ast_float, */
        /*ast_string, */
        ast_ident,
        ast_type,
        ast_function_type,
        ast_pointer_type,

        ast_block,
        ast_declaration,
        ast_assignment,
        ast_return,

        /* unimplemnted: */
        ast_call,
        ast_unary_op,
        ast_binary_op
    } type;
    union {
        long int integer;
        struct {
            char *name;
        } ident;
        struct {
            char *name;
        } type;
        struct {
            struct AstExp *to;
        } pointer_type;
        struct {
            struct AstExp *return_type;
            struct AstExp *parameters;
            /* See note in ast_block for parameter handling */
            /* functions are represented as:
             * declaration
             *  - type = function_type
             *            - return_type
             *            - parameters
             *  - name = (name)
             *  - v    = ast_block of statements.
             */
        } function_type;
        struct {
            struct AstExp *type;
            struct AstExp *ident;
            struct AstExp *value;
        } declaration;
        struct {
            struct AstExp *ident;
            struct AstExp *value;
        } assignment;
        struct {
            struct AstExp *expression;
            struct AstExp *rest;
            /* For now:
             *
             * Blocks will be represented as a linked list of
             * statements.  This has the advantage that each one
             * can be represented as a new scope, and not have to
             * deal with crazy scope resolution problems.
             *
             * It also allows blocks (and function arguments, which
             * are similar but restricted to only declarations,
             * as opposed to arbitrary AstExps) to be represented
             * as a single AstExp with no crazyness.
             *
             * This has the added benefit that no where is there
             * an arbitrary sized pointer (arrays of things),
             * and I should only need to traverse the tree downwards,
             * so I don't expect any adverse effects.
             */
            
            /* For now:
             * 
             * Let's try using block for function argument lists
             * and manually restricting to declarations.  If this
             * becomes unweildy, I will add another type or make
             * some customerizations to ast_function.
             */
        } block;
    } v;

    size_t line_number;
    size_t line_index;
    char *filename;

    int id;
    /* id will be used for type and identifier
     * resolution in the scope resolution pass
     * and thereafter.
     *
     * It will uniquely identify each object
     * as soon as it is established.  Potentially
     * just by incrementing for every new reference.
     *
     * 0 will be special and mean unknown/unresolved.
     *
     * Always initialize this to 0 to prevent bad
     * data from sneaking in.
     */
} AstExp;

AstExp *make_integer_node(long int value);
AstExp *make_ident_node(char *name);
AstExp *make_type_node(char *name);
AstExp *make_pointer_type_node(AstExp *to);

#endif

