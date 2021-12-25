#include <stdlib.h>
#include "internal_trie.h"
#include "../misc/util.h"


/*
 * This trie is implemented with linked lists.
 * Each trie node is contained in a linked list and has a linked list which contains it's child nodes.
 * To get the next child of this trie node's parent, follow the sibling pointer.
 * To get the first child of this trie node, follow the child pointer.
 * This implementation does not have the best time complexity compared to an array implementation,
 * but as our alphabet is rather large (all possible byte values = 256) and the expected branching
 * factor is quite low, it saves a lot of memory. In practice, a single node will often have
 * very few children, which makes the search for the right child faster than you might expect.
 * This is mostly only false for the absolute root of the trie, which will often have almost the maximum
 * amount of children, as adding labels of length 1 is very common. This problem is solved in the tree
 * with the byte_to_trie variable, which uses the normal array implementation for the root of the trie,
 * while all other nodes use the linked list approach.
 */
typedef struct InternalTrie {
    unsigned char value;
    Node* associated_node;
    struct InternalTrie* sibling;
    struct InternalTrie* child;
} InternalTrie;


InternalTrie* init_internal_trie(unsigned char value, Node* node) {
    InternalTrie* trie = safe_malloc(sizeof(InternalTrie));
    trie->value = value;
    trie->associated_node = node;
    trie->sibling = NULL;
    trie->child = NULL;
    return trie;
}


// Recursively free a trie node's siblings, then it's children and finally itself.
void free_internal_trie(InternalTrie* trie) {
    if (trie != NULL) {
        free_internal_trie(trie->sibling);
        free_internal_trie(trie->child);
        free(trie);
    }
}


Node* search_label(InternalTrie* trie, Label* label) {
    InternalTrie* current_trie_node = trie;
    for (int i = 1; i < label->length; i++) {
        if (current_trie_node == NULL) {
            return NULL;
        }
        current_trie_node = current_trie_node->child;
        unsigned char ch = label->value[i];
        while (current_trie_node != NULL && current_trie_node->value != ch) {
            current_trie_node = current_trie_node->sibling;
        }
    }
    return current_trie_node == NULL ? NULL : current_trie_node->associated_node;
}


size_t add_to_internal_trie(InternalTrie* trie, Node* new_leaf_node) {
    InternalTrie* previous_trie = trie;
    InternalTrie* current_trie = trie->child;
    int len = new_leaf_node->label->length;
    if (current_trie == NULL && len > 1) {
        current_trie = init_internal_trie(new_leaf_node->label->value[1], NULL);
        previous_trie->child = current_trie;
    }
    size_t allocated_bytes = 0;
    for (int i = 1; i < len; i++) {
        unsigned char ch = new_leaf_node->label->value[i];
        while (current_trie != NULL && current_trie->value != ch) {
            previous_trie = current_trie;
            current_trie = current_trie->sibling;
        }
        if (current_trie == NULL) {
            current_trie = init_internal_trie(ch, NULL);
            previous_trie->sibling = current_trie;
            allocated_bytes += sizeof(InternalTrie);
        }
        previous_trie = current_trie;
        current_trie = current_trie->child;
        if (current_trie == NULL && i+1 < len) {
            current_trie = init_internal_trie(new_leaf_node->label->value[i+1], NULL);
            previous_trie->child = current_trie;
            allocated_bytes += sizeof(InternalTrie);
        }
    }
    previous_trie->associated_node = new_leaf_node;
    return allocated_bytes;
}
