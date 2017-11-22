
#include <stdio.h>

#include "vector.h"

struct Test {
    int foo;
    char* bar;
};

int main()
{
    int i;
    int* f;

    Vector* v1 = new_vec(int);
    Vector* v2 = new_vec(int);

    print_vector(v1);
    print_vector(v2);

    for (i = 0; i < 10000000; i++) {
        vec_push(v1, &i);
        vec_push(v2, &i);
    }

    print_vector(v1);
    print_vector(v2);

    /*
    Vector *h = new_vec(struct Test);
    struct Test t = {100, "Hello"};
    vec_push(h, &t);
    struct Test *pcopyt = vec_get(h, 0);

    print_vector(h);
    printf("t.foo = %i, t.bar = \"%s\"\n", pcopyt->foo, pcopyt->bar);
    */
}
