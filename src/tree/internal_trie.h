#ifndef PROJECT_INTERNAL_TRIE_H
#define PROJECT_INTERNAL_TRIE_H

#include "node.h"

/*
 * This trie stores all labels that have been added to the tree.
 * The differences between this InternalTrie struct and the Trie struct are that this struct
 * 1) is permanent as it is only freed at the end of the encode method, while
 *    the Trie struct is temporary as it is freed at every new buffer read.
 * 2) stores labels that have actually been added to the tree, while
 *    the Trie struct stores all substrings (<= max_label_length) of the buffer.
 */
typedef struct InternalTrie InternalTrie;

/*
 * Returns a new node in the internal trie. It is initialised with the value and node parameters.
 * It is possible for node to be NULL. In this case the trie node is only an internal node,
 * and does not have an associated node in the tree.
 */
InternalTrie* init_internal_trie(unsigned char value, Node* node);

// Given the root node of the trie, free the entire trie.
void free_internal_trie(InternalTrie* root);

/*
 * Returns the node associated with the given label,
 * or NULL if no node in the tree has this label.
 */
Node* search_label(InternalTrie* trie, Label* label);

/*
 * Add the label of the given node to the trie.
 * Returns the additional amount of bytes allocated to accomplish this.
 */
size_t add_to_internal_trie(InternalTrie* trie, Node* new_leaf_node);

#endif //PROJECT_INTERNAL_TRIE_H
