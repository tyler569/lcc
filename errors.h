
#ifndef ERRORS_H
#define ERRORS_H

#include "loc.h"

void lcc_warning(Location, char*);
void lcc_error(Location, char*);
void lcc_compiler_error_internal(char*, size_t, char*);

#define lcc_compiler_error(message) \
    lcc_compiler_error_internal(__FILE__, __LINE__, message)

#endif
