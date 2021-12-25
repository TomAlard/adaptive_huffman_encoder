#ifndef PROJECT_TREE_H
#define PROJECT_TREE_H

#include "node.h"
#include "../misc/bit_stream.h"

// The adaptive huffman tree.
typedef struct Tree Tree;

Tree* create_tree();

/*
 * Given a label, return the node associated with this label,
 * or NULL if there is no such node.
 */
Node* get_leaf_node_from_label(Tree* tree, Label* label);

/*
 * Given the node and the label that was used in the encoding/decoding process,
 * adjust the tree by increasing the node's weight and swapping nodes.
 */
void adjust_tree(Tree* tree, Node* node, Label* label);

Node* get_root(Tree* tree);

Node* get_nng(Tree* tree);

size_t get_tree_allocated_bytes(Tree* tree);

void free_tree(Tree* tree);

#endif //PROJECT_TREE_H
