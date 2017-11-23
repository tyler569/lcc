
#ifndef LOC_H
#define LOC_H

typedef struct Location {
    char *filename;
    size_t line;
    size_t index;
} Location;

#endif
