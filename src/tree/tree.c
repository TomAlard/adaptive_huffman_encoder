#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "../misc/util.h"
#include "node_array_list.h"
#include "internal_trie.h"


/*
 * Stores direct references to the root and nng nodes, and the id_plus variable,
 * which is added with the id variable found in nodes to obtain the real id of a node.
 * This struct also stores the root node of it's InternalTrie struct. For more information on
 * why this is done, see the documentation for the InternalTrie struct.
 * Finally the tree also stores an Array(List) containing the nodes sorted from largest id to smallest id,
 * and a variable allocated_bytes which indicates how many bytes of memory the tree is using.
 */
typedef struct Tree {
    Node* root;
    Node* nng;
    int id_plus;
    InternalTrie** byte_to_trie;
    NodeArrayList* node_array_list;
    size_t allocated_bytes;
} Tree;


Tree* create_tree() {
    Node* nng = create_nng();
    Tree* tree = safe_malloc(sizeof(Tree));
    tree->root = nng;
    tree->nng = nng;
    tree->id_plus = 0;
    InternalTrie** byte_to_trie = safe_malloc(256 * sizeof(InternalTrie*));
    for (int i = 0; i < 256; i++) {
        byte_to_trie[i] = NULL;
    }
    tree->byte_to_trie = byte_to_trie;
    tree->node_array_list = create_node_array_list();
    tree->allocated_bytes = sizeof(Tree) + (256 * sizeof(InternalTrie*)) + get_node_array_list_allocated_bytes(tree->node_array_list);
    return tree;
}


Node* get_leaf_node_from_label(Tree* tree, Label* label) {
    return search_label(tree->byte_to_trie[label->value[0]], label);
}


void add_new_node(Tree* tree, Label* label) {
    tree->id_plus += 2;
    Node* new_internal_node = add_new_leaf_node(tree->nng, tree->id_plus, label);
    Node* new_leaf_node = new_internal_node->right;
    tree->allocated_bytes += (2 * sizeof(Node)) + sizeof(Label);
    if (new_internal_node->parent == NULL) {
        tree->root = new_internal_node;
    }
    unsigned char first_char = new_leaf_node->label->value[0];
    InternalTrie* first_char_trie = tree->byte_to_trie[first_char];
    if (first_char_trie == NULL) {
        Node* associated_node = new_leaf_node->label->length == 1? new_leaf_node : NULL;
        first_char_trie = init_internal_trie(first_char, associated_node);
        tree->byte_to_trie[first_char] = first_char_trie;
    }
    tree->allocated_bytes += add_to_internal_trie(first_char_trie, new_leaf_node);
    insert_node(tree->node_array_list, new_internal_node);
    insert_node(tree->node_array_list, new_leaf_node);
}


Node* find_max_id_node(Tree* tree, Node* node) {
    return find_leader(tree->node_array_list, node, tree->id_plus);
}


void adjust_tree(Tree* tree, Node* node, Label* label) {
    Node* adjust_node = node;
    if (adjust_node == NULL) {
        add_new_node(tree, label);
        adjust_node = tree->nng->parent->parent;
        if (adjust_node == NULL) {
            return;
        }
    }

    while (adjust_node != tree->root) {
        Node* max_id_node = find_max_id_node(tree, adjust_node);
        if (max_id_node != adjust_node && adjust_node->parent != max_id_node && max_id_node->parent != adjust_node) {
            notify_swap(tree->node_array_list, max_id_node, adjust_node, tree->id_plus);
            swap_nodes(max_id_node, adjust_node);
        }
        increment_node_weight(adjust_node);
        adjust_node = adjust_node->parent;
    }
    increment_node_weight(adjust_node);
}


Node* get_root(Tree* tree) {
    return tree->root;
}


Node* get_nng(Tree* tree) {
    return tree->nng;
}


size_t get_tree_allocated_bytes(Tree* tree) {
    return tree->allocated_bytes + get_node_array_list_allocated_bytes(tree->node_array_list);
}


void free_tree(Tree* tree) {
    for (int i = 0; i < 256; i++) {
        free_internal_trie(tree->byte_to_trie[i]);
    }
    free(tree->byte_to_trie);
    free_node_array_list(tree->node_array_list);
    free_nodes(tree->root);
    free(tree);
}
