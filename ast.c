
#include <stdlib.h>

#include "errors.h"
#include "typer.h"
#include "token.h"
#include "ast.h"

AstExp *make_integer_node(long int value) {
    AstExp *result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_integer;
    result->v.integer = value;

    return result;
}

AstExp *make_ident_node(char *name) {
    AstExp *result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_ident;
    result->v.ident.name = name;

    return result;
}

AstExp *make_type_node(char *name) {
    AstExp *result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_type;
    result->v.type.name = name;

    return result;
}

AstExp *make_pointer_type_node(AstExp *to) {
    AstExp *result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_pointer_type;
    result->v.pointer_type.to = to;

    return result;
}

AstExp *make_function_type_node(AstExp *ret, AstExp *params) {
    AstExp *result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_pointer_type;
    result->v.function_type.return_type = ret;
    result->v.function_type.parameters = params;

    return result;
}

AstExp *make_declaration_node(AstExp *type, AstExp *ident, AstExp *value) {
    AstExp *result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_pointer_type;
    result->v.declaration.type = type;
    result->v.declaration.ident = ident;
    result->v.declaration.value = value;

    return result;
}


AstExp *make_ast_from_tokens(TokenList *t) {
    AstExp *result = malloc(sizeof(AstExp));

    result->id = 0;
    result->type = ast_block;
    
    /* parse one expression */

    switch (t->v->type) {
    case token_ident:
        if (!ident_is_type(t->v->value.ident)) {
            /* scope resolution eventually */
            lcc_error(t->v->line_number, t->v->line_index, "Unknown idetifier");
        }
        /* it's a declaration */
        /* resolve multiword type
         *   (unsigned char, struct foobar)
         */

        /* @Bug: pointer resolution goes here */
        if (t->next->v->type != token_ident) {
            lcc_error(t->v->line_number, t->v->line_index, "Only identifiers can be declared");
        }

        if (t->next->next->v->type == token_open_paren) {
            /* We're a function, yippie */
            /* TODO: arguments */
            AstExp *ret_type = make_type_node(t->v->value.ident);
            AstExp *params = NULL;
            AstExp *block = NULL;
            AstExp *fn_type = make_function_type_node(ret_type, params);
        }


    default:
        lcc_error(t->v->line_number, t->v->line_index, "Invalid token");
    }

    /* recurse at a later point in token stream */
    /* Can I do this with a while? I think probably.*/

    return result;
}
