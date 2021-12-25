#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "util.h"


// From https://stackoverflow.com/a/47990
int set_bit(int x, int bit_value, int bit_index) {
    return x ^ ((-bit_value ^ x) & (1 << bit_index));
}


// From https://stackoverflow.com/a/47990
int get_bit(int x, int bit_index) {
    return (x >> bit_index) & 1;
}


void my_assert(bool condition, char* msg) {
    if (!condition) {
        fprintf(stderr, "%s\n", msg);
        exit(1);
    }
}


void* safe_malloc(size_t n) {
    void* p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "safe_malloc: Couldn't allocate %zu bytes.\n", n);
        exit(1);
    }
    return p;
}


void* safe_realloc(void* ptr, size_t n) {
    void* new_ptr = realloc(ptr, n);
    if (new_ptr == NULL) {
        fprintf(stderr, "safe_realloc: Couldn't allocate %zu bytes.\n", n);
        exit(1);
    }
    return new_ptr;
}


int min_(int a, int b) {
    return a < b? a : b;
}
