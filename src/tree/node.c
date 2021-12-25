#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "../misc/util.h"


Node* create_node(int id, int weight, Label* label, Node* left, Node* right, Node* parent) {
    Node* node = safe_malloc(sizeof(Node));
    node->id = id;
    node->weight = weight;
    node->label = label;
    node->left = left;
    node->right = right;
    node->parent = parent;
    return node;
}

Node* create_internal_node(int id, Node* left, Node* right, Node* parent) {
    return create_node(id, 1, NULL, left, right, parent);
}

Node* create_leaf_node(int id, Label* label, Node* parent) {
    Label* label_copy = safe_malloc(sizeof(Label));
    label_copy->length = label->length;
    label_copy->value = safe_malloc(label_copy->length);
    memcpy(label_copy->value, label->value, label_copy->length);
    return create_node(id, 1, label_copy, NULL, NULL, parent);
}

Node* create_nng() {
    return create_node(0, 0, NULL, NULL, NULL, NULL);
}


// Recursively free the left child, then the right child and finally itself.
void free_nodes(Node* node) {
    if (node != NULL) {
        free_nodes(node->left);
        free_nodes(node->right);
        if (node->label != NULL) {
            free(node->label->value);
            free(node->label);
        }
        free(node);
    }
}


void swap_nodes(Node* n, Node* m) {
    // determine if left or right of parent
    int n_is_left = n->parent->left == n;
    int m_is_left = m->parent->left == m;

    // swap left or right of parent
    if (n_is_left) n->parent->left = m;
    else n->parent->right = m;
    if (m_is_left) m->parent->left = n;
    else m->parent->right = n;

    // swap parents of n and m
    Node* n_parent = n->parent;
    n->parent = m->parent;
    m->parent = n_parent;

    // swap id's of n and m
    int n_id = n->id;
    n->id = m->id;
    m->id = n_id;
}


Node* add_new_leaf_node(Node* nng, int id_plus, Label* label) {
    Node* new_leaf_node = create_leaf_node(1 - id_plus, label, NULL);
    Node* new_internal_node = create_internal_node(2 - id_plus, nng, new_leaf_node, nng->parent);
    new_leaf_node->parent = new_internal_node;
    nng->id = -id_plus;
    if (nng->parent != NULL) {
        nng->parent->left = new_internal_node; // nng is always left child
    }
    nng->parent = new_internal_node;
    return new_internal_node;
}


int get_node_id(Node* node, int id_plus) {
    return node->id + id_plus;
}

void increment_node_weight(Node* node) {
    node->weight++;
}

bool is_leaf_node(Node* node) {
    return node->left == NULL && node->right == NULL;
}

bool has_same_weight(Node* n, Node* m) {
    return n->weight == m->weight;
}
