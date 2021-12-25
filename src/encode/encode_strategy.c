#include "encode_strategy.h"
#include "../misc/util.h"


char should_encode_string(Tree* tree, Trie* trie, Label* label, bool can_allocate_memory) {
    if (label->length == 1 || get_leaf_node_from_label(tree, label) != NULL) {
        return 127;
    }
    if (!can_allocate_memory) {
        return -1;
    }
    // It is acceptable to encode any label that occurs more than once in the buffer.
    int occurrences = get_label_occurrences(trie, label);
    return occurrences > 1? occurrences - 2 : -1;
}
