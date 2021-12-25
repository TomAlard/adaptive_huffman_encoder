#ifndef PROJECT_UTIL_H
#define PROJECT_UTIL_H

#include <stdio.h>
#include "../tree/tree.h"

// x[bit_index] = bit_value
int set_bit(int x, int bit_value, int bit_index);

// return x[bit_index]
int get_bit(int x, int bit_index);

// Exit the program if the condition is false with the given message.
void my_assert(bool condition, char* msg);

// Attempt to malloc n bytes, exit the program if this failed.
void* safe_malloc(size_t n);

// Attempt to realloc n bytes, exit the program if this failed.
void* safe_realloc(void* ptr, size_t n);

int min_(int a, int b);

#endif //PROJECT_UTIL_H
