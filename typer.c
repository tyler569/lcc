
#include <stdbool.h>
#include <string.h>

#include "typer.h"

typedef enum DefaultType {
    type_invalid = 0,
    type_int /* I only support ints for now */
} DefaultType;

typedef struct TypenameMapping {
    DefaultType id;
    char *name;
} TypenameMapping;

TypenameMapping default_type_mappings[] = {
    {type_int, "int"}
};

/* @HACK
 * just checks to see if it's "int"
 *
 * DOES NOT USE ANY OTHER INFRASTRUCTURE MADE HERE
 *
 * FIX ME
 */
bool ident_is_type(char *name) {
    if (strcmp(name, "int") == 0) {
        return true;
    } else {
        return false;
    }
}
