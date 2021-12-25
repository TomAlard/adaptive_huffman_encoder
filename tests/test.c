#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/encode/encode.h"
#include "../src/misc/util.h"


void test_is_optimal_tree(Node* root);
Tree* create_test_tree() {
    Tree* tree = create_tree();
    Label* x = safe_malloc(sizeof(Label));
    x->value = (unsigned char*)"xtreme";
    x->length = 6;
    Label* y = safe_malloc(sizeof(Label));
    y->value = (unsigned char*)"ynot";
    y->length = 4;
    Label* z = safe_malloc(sizeof(Label));
    z->value = (unsigned char*)"zuckerberg";
    z->length = 10;
    adjust_tree(tree, NULL, x);
    adjust_tree(tree, NULL, y);
    adjust_tree(tree, get_leaf_node_from_label(tree, y), y);
    adjust_tree(tree, NULL, z);
    test_is_optimal_tree(get_root(tree));
    return tree;
}


void test_get_leaf_node_from_label() {
    Tree* tree = create_test_tree();
    my_assert(get_leaf_node_from_label(tree, get_root(tree)->left->right->label) == get_root(tree)->left->right,
              "test_get_leaf_node_from_label: couldn't find x");
    my_assert(get_leaf_node_from_label(tree, get_root(tree)->right->label) == get_root(tree)->right,
              "test_get_leaf_node_from_label: couldn't find y");
    my_assert(get_leaf_node_from_label(tree, get_root(tree)->left->left->right->label) == get_root(tree)->left->left->right,
              "test_get_leaf_node_from_label: couldn't find z");
    free_tree(tree);
}


int rule_a_is_valid(Node* node) {
    if (is_leaf_node(node)) return 1;

    return node->left->weight + node->right->weight == node->weight
            ? rule_a_is_valid(node->left) && rule_a_is_valid(node->right)
            : 0;
}


int rule_b_is_valid_for_given_node(Node* given_node, Node* current_node) {
    if (current_node == NULL) return 1;
    if (given_node->id < current_node->id && given_node->weight > current_node->weight) return 0;
    return rule_b_is_valid_for_given_node(given_node, current_node->left)
        && rule_b_is_valid_for_given_node(given_node, current_node->right);
}


int rule_b_is_valid(Node* root, Node* current_node) {
    if (current_node == NULL) return 1;
    return rule_b_is_valid(root, current_node->left)
        && rule_b_is_valid_for_given_node(current_node, root)
        && rule_b_is_valid(root, current_node->right);
}


int rule_c_is_valid(Node* node) {
    if (is_leaf_node(node)) return 1;
    if (abs(node->left->id - node->right->id) != 1) return 0;
    return rule_c_is_valid(node->left)
        && rule_c_is_valid(node->right);
}


void test_is_optimal_tree(Node* root) {
    my_assert(rule_a_is_valid(root), "test_is_optimal_tree: rule a invalid (invalid weights)");
    my_assert(rule_b_is_valid(root, root), "test_is_optimal_tree: rule b invalid (invalid id-weights relation)");
    my_assert(rule_c_is_valid(root), "test_is_optimal_tree: rule c invalid (invalid siblings id)");
}


// Test some toy examples and check that the tree remains optimal.
void test_adjust_tree() {
    char test_strings[3][10] = {"rake_zaak", "voordeel", "boosdoeer"};
    for (int i = 0; i < 3; i++) {
        Tree* tree = create_tree();
        for (int j = 0; j < strlen(test_strings[i]); j++) {
            unsigned char* value = (unsigned char*) &test_strings[i][j];
            Label label = {value, 1};
            adjust_tree(tree, get_leaf_node_from_label(tree, &label), &label);
            test_is_optimal_tree(get_root(tree));
        }
        free_tree(tree);
    }
}


void run_tests() {
    test_get_leaf_node_from_label();
    test_adjust_tree();
}