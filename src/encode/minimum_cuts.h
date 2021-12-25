#ifndef PROJECT_MINIMUM_CUTS_H
#define PROJECT_MINIMUM_CUTS_H

#include <stdbool.h>
#include "encode_strategy.h"

/*
 * Given a string s, return an array containing indices of characters in the string s, such that
 * cutting the string into substrings at these indices would result in
 * a minimal partition of the string into labels which are either of length 1 OR already present
 * in the tree OR common enough that they should be added to the tree.
 */
unsigned char* get_cuts(unsigned char* result_first_cut_index, Tree* tree, char* s, int s_length, int max_label_length,
                        bool can_allocate_memory);

#endif //PROJECT_MINIMUM_CUTS_H
