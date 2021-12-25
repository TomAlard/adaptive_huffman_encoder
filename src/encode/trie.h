#ifndef PROJECT_TRIE_H
#define PROJECT_TRIE_H

#include "../tree/node.h"

/*
 * This trie stores the amount of occurrences of all substrings of a given string s in build_trie.
 * For more information on how this differs from the InternalTrie struct,
 * see the InternalTrie struct documentation.
 */
typedef struct Trie Trie;

// Given the root node of the trie, free the entire trie.
void free_trie(Trie* root);

// Returns the amount of times this label occurred in the original string s.
int get_label_occurrences(Trie* trie, Label* label);

// Build the trie by adding all substrings (<= max_label_length) of s to an initially empty trie.
Trie* build_trie(char* s, int len, int max_label_length);

#endif //PROJECT_TRIE_H
