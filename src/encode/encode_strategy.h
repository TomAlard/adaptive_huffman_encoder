#ifndef PROJECT_ENCODE_STRATEGY_H
#define PROJECT_ENCODE_STRATEGY_H

#include <stdbool.h>
#include "../tree/tree.h"
#include "trie.h"

/*
 * Return whether it would be acceptable to encode the given label as a separate node.
 * This is true if the label only has a length of 1, there is already a node with this label in the tree
 * or the label occurs often enough in the string we wish to encode (the label is common)
 * AND we are allowed to allocate new memory. A value of -1 signifies we do not wish to encode
 * this label at all. A value >= 0 means that it would be acceptable to encode this label,
 * but it is not yet present in the tree, so the encoder would have to add it to the tree.
 * Finally, a value of 127 means that the label is already present in the tree. This is the most optimal situation.
 * The different values are used to break ties when determining the best partition of a string
 * (see the is_better_partition function in minimum_cuts.c)
 */
char should_encode_string(Tree* tree, Trie* root, Label* label, bool can_allocate_memory);

#endif //PROJECT_ENCODE_STRATEGY_H
