
#ifndef ERRORS_H
#define ERRORS_H

void lcc_warning(int line, int ix, char *message);
void lcc_error(int line, int ix, char *message);
void lcc_compiler_error_internal(char *file, int line, char *message);

#define lcc_compiler_error(message) lcc_compiler_error_internal(__FILE__, __LINE__, message)

#endif

