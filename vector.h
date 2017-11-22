
#ifndef VECTOR_H
#define VECTOR_H

typedef enum Strategy {
    strategy_by_1,
    strategy_by_16,
    strategy_by_256,
    strategy_x2,
    strategy_x4,
} Strategy;

typedef struct Vector {
    size_t len;
    size_t total_size;
    size_t delta;
    Strategy strategy;
    void* data;
} Vector;

Vector* new_vec_internal(size_t count, size_t delta, Strategy strategy);
#define new_vec(type) new_vec_internal(16, sizeof(type), strategy_x2)
#define new_vec_s(type, strat) new_vec_internal(16, sizeof(type), strat)

void vec_set(Vector* vec, size_t index, void* value);
void vec_push(Vector* vec, void* value);
void* vec_get(Vector* vec, size_t index);

void print_vector(Vector*);

#endif
