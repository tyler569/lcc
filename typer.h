
#ifndef TYPER_H
#define TYPER_H

#include <stdbool.h>

typedef int type_id;
#define NOT_A_TYPE (type_id)0

typedef struct Type {
    char* name;
    int id;
} Type;

typedef struct TypeList {
    Type v;
    struct TypeList* next;
} TypeList;

bool ident_is_type(char* name);

type_id register_new_typedef(char* name);

/* Structure and arguments to be determined */
type_id new_struct();
type_id new_enum();
type_id new_union();

#endif
