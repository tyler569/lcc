
#include <stdlib.h>

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
