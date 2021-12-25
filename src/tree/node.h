#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

#include <stdbool.h>


/*
 * Contains the label of a leaf node. As value could contain \0 bytes,
 * we cannot simply store it as is, since we would not be able to determine where it ends.
 * Therefore we also store the length of value.
 */
typedef struct Label {
    unsigned char* value;
    unsigned char length;
} Label;

/*
 * A single node in the tree. For leaf nodes, left and right are NULL.
 * For internal nodes, label is NULL. For nng, id and weight are kept at 0, while
 * label, left and right are NULL. nng is considered to be a leaf node.
 * The id stored in the node is not the real id,
 * as we first have to add it with the id_plus variable stored in the associated Tree struct.
 */
typedef struct Node {
    int id;
    int weight;
    Label* label;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

Node* create_internal_node(int id, Node* left, Node* right, Node* parent);

// This copies the given label, so the caller of this method should free label afterwards.
Node* create_leaf_node(int id, Label* label, Node* parent);

Node* create_nng();

// Given the root node of the tree, free all nodes.
void free_nodes(Node* root);

// Swaps two nodes in the tree.
void swap_nodes(Node* n, Node* m);

/*
 * Add a new label to the tree by replacing nng in the tree with a new internal node
 * which has nng on it's left and a new leaf node containing the new label on it's right.
 */
Node* add_new_leaf_node(Node* nng, int id_plus, Label* label);

// Get the real id of a node given the associated tree's id_plus.
int get_node_id(Node* node, int id_plus);

void increment_node_weight(Node* node);

/*
 * Returns whether a node is a leaf node (true) or an internal node (false).
 * nng is considered to be a leaf node.
 */
bool is_leaf_node(Node* node);

bool has_same_weight(Node* n, Node* m);

#endif //PROJECT_NODE_H
