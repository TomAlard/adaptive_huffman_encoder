#include <stdlib.h>
#include "trie.h"
#include "../misc/util.h"


typedef struct Trie {
    unsigned char value;
    int occurrences;
    struct Trie* sibling;
    struct Trie* child;
} Trie;


Trie* init_trie(unsigned char value) {
    Trie* trie = safe_malloc(sizeof(Trie));
    trie->value = value;
    trie->occurrences = 0;
    trie->sibling = NULL;
    trie->child = NULL;
    return trie;
}


// Recursively free a trie node's siblings, then it's children and finally itself.
void free_trie(Trie* trie) {
    if (trie != NULL) {
        free_trie(trie->sibling);
        free_trie(trie->child);
        free(trie);
    }
}


int get_label_occurrences(Trie* root, Label* label) {
    Trie* current_node = root;
    for (int i = 0; i < label->length; i++) {
        unsigned char ch = label->value[i];
        while (current_node->value != ch) {
            current_node = current_node->sibling;
        }
        if (i+1 < label->length) {
            current_node = current_node->child;
        }
    }
    return current_node->occurrences;
}


Trie* find_next_sibling(Trie* node, unsigned char ch) {
    Trie* current_node = node;
    Trie* previous_node = NULL;
    while (current_node != NULL && current_node->value != ch) {
        previous_node = current_node;
        current_node = current_node->sibling;
    }
    return previous_node;
}


Trie* build_trie(char* s, int len, int max_label_length) {
    Trie* root = init_trie(s[0]);
    for (int i = 0; i < len; i++) {
        Trie* current_node = root;
        for (int j = 0; j < max_label_length && i+j < len; j++) {
            unsigned char ch = s[i+j];
            Trie* previous_sibling = find_next_sibling(current_node, ch);
            Trie* found = previous_sibling == NULL? current_node : previous_sibling->sibling;
            if (found == NULL) {
                previous_sibling->sibling = init_trie(ch);
                found = previous_sibling->sibling;
            }
            found->occurrences++;
            if (j+1 < max_label_length && i+j+1 < len) {
                if (found->child == NULL) {
                    found->child = init_trie(s[i+j+1]);
                }
                current_node = found->child;
            }
        }
    }
    return root;
}