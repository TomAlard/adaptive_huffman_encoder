#include <stdlib.h>
#include "decode.h"
#include "../tree/tree.h"
#include "../misc/util.h"


/*
 * Decode the path to a leaf node. Return the label of that leaf node.
 * If the leaf node found is nng, the resulting label will be NULL, as
 * nng is the only leaf node which does not have a label.
 */
Label* decode_path(BitStream* bit_stream, Tree* tree) {
    Node* current_node = get_root(tree);
    while (!is_leaf_node(current_node)) {
        bool bit = read_bit(bit_stream);
        current_node = bit? current_node->right : current_node->left;
    }
    return current_node->label;
}


/*
 * Reads a new label. The first byte 'amount' indicates the amount
 * of bytes the label consists of, then that many bytes are read.
 */
Label* decode_new_label(BitStream* bit_stream) {
    unsigned char amount = read_byte(bit_stream);
    if (amount == 0) {
        return NULL;
    }

    Label* label = safe_malloc(sizeof(Label));
    label->length = amount;
    label->value = safe_malloc(label->length);
    for (int i = 0; i < label->length; i++) {
        label->value[i] = read_byte(bit_stream);
    }
    return label;
}


/*
 * Decode the path to a leaf node. If this leaf node is nng, decode a new label first.
 * Then output that label to the output_file and pass it to adjust_tree to keep this method's tree
 * in sync with the encoder.
 */
void decode(FILE* input_file, FILE* output_file) {
    Tree* tree = create_tree();
    BitStream* bit_stream = init_read_bit_stream(input_file);
    while (1) {
        bool free_label = false;
        Label* next_label = decode_path(bit_stream, tree);
        if (next_label == NULL) {
            next_label = decode_new_label(bit_stream);
            free_label = true;
            if (next_label == NULL) {
                break;
            }
        }
        fwrite(next_label->value, 1, next_label->length, output_file);
        adjust_tree(tree, get_leaf_node_from_label(tree, next_label), next_label);
        if (free_label) {
            free(next_label->value);
            free(next_label);
        }
    }
    free_tree(tree);
    free(bit_stream);
}