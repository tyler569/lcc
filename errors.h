
#ifndef ERRORS_H
#define ERRORS_H

void lcc_warning(size_t line, size_t ix, char *message);
void lcc_error(size_t line, size_t ix, char *message);
void lcc_compiler_error_internal(char *file, size_t line, char *message);

#define lcc_compiler_error(message) lcc_compiler_error_internal(__FILE__, __LINE__, message)

#endif

