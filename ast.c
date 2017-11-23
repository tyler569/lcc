
#include <stdlib.h>

#include "ast.h"
#include "errors.h"
#include "token.h"
#include "typer.h"

AstExp* make_integer_node(long int value)
{
    AstExp* result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_integer;
    result->v.integer = value;

    return result;
}

AstExp* make_ident_node(char* name)
{
    AstExp* result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_ident;
    result->v.ident.name = name;

    return result;
}

AstExp* make_type_node(char* name)
{
    AstExp* result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_type;
    result->v.type.name = name;

    return result;
}

AstExp* make_pointer_type_node(AstExp* to)
{
    AstExp* result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_pointer_type;
    result->v.pointer_type.to = to;

    return result;
}

AstExp* make_function_type_node(AstExp* ret, AstExp* params)
{
    AstExp* result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_pointer_type;
    result->v.function_type.return_type = ret;
    result->v.function_type.parameters = params;

    return result;
}

AstExp* make_declaration_node(AstExp* type, AstExp* ident, AstExp* value)
{
    AstExp* result = malloc(sizeof(AstExp));
    result->id = 0;

    result->type = ast_pointer_type;
    result->v.declaration.type = type;
    result->v.declaration.ident = ident;
    result->v.declaration.value = value;

    return result;
}

// AstExp* make_ast_from_tokens(TokenList* t) { return NULL; }

/* So let's think about this garbage.
 *
 * I need a way to compartmentalize this into something sane.
 *
 * I'm thinking lots of subfunctions that are looking to parse
 * individual specific things given a token stream, similar
 * to the way I programmed the tokenizer for this compiler.
 *
 * As example:
 *  Have some function to match a series of token types
 *   bool token_pattern_match(TokenList *, <something>)
 *  This allows me to ask questions like
 *   "Do we have a type, then a name, then a paren?"
 *   "Do we have a type, then a name, then an =?"
 *   "Do we have a type, then a name, then a ;"
 *  and pass those cases to different parse_* subfns.
 *
 * There is other infrastructure for this required as well.
 * I will need to be able to resolve what is a type, and
 * be able to combine multiword types in a sane way.
 *
 * This is all trivially easy for starters since I only
 * support 'int' for the moment, but going forward it
 * will need to be done in some way.
 */
