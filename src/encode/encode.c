#include <stdio.h>
#include <stdlib.h>
#include "../tree/tree.h"
#include "../misc/util.h"
#include "minimum_cuts.h"


/*
 * Encodes the path from the root of the tree to the given node.
 * To indicate the node is on the left of the current node we write a 0 bit.
 * To indicate it's on the right we write a 1 bit.
 */
void encode_path(BitStream* bit_stream, Node* node) {
    if (node->parent != NULL) {
        encode_path(bit_stream, node->parent);
        bool is_right = node->parent->right == node;
        write_bit(bit_stream, is_right);
    }
}


/*
 * Encode the amount of characters in the label as one byte.
 * Then encode each character (byte) in the label.
 */
void encode_new_label(BitStream* bit_stream, Label* label) {
    write_byte(bit_stream, label->length);
    for (int i = 0; i < label->length; i++) {
        write_byte(bit_stream, label->value[i]);
    }
}


/*
 * Encodes a label. If there is no leaf node with that label, encode the path
 * to nng followed by the amount of characters in the label and then encode each character separately.
 * If the leaf node already exists, encode the path to the leaf node.
 */
void encode_label(BitStream* bit_stream, Tree* tree, Node* node, Label* label) {
    if (node == NULL) {
        encode_path(bit_stream, get_nng(tree));
        encode_new_label(bit_stream, label);
    } else {
        encode_path(bit_stream, node);
    }
}


/*
 * To indicate the end of the file we must write the path to nng followed by a zero byte.
 * We then also pad the last byte with zero's until it contains 8 bits.
 */
void encode_end(BitStream* bit_stream, Tree* tree) {
    encode_path(bit_stream, get_nng(tree));
    write_byte(bit_stream, 0);
    pad_byte_with_zero_bits(bit_stream);
}


/*
 * Encodes a string s of given length s_length. This is done by first getting
 * the minimal partition of the string into valid substrings (see get_cuts).
 * Then each substring (=label) is passed to encode_label.
 * Finally adjust_tree is called with the new label.
 */
void encode_string(BitStream* bit_stream, Tree* tree, char* s, int s_length, int max_label_length, bool can_allocate_memory) {
    unsigned char first_cut_index;
    unsigned char* cuts = get_cuts(&first_cut_index, tree, s, s_length, max_label_length, can_allocate_memory);
    int i = first_cut_index;
    while (i < s_length) {
        int start = cuts[i];
        int end = (i == s_length-1)? s_length : cuts[i+1];
        unsigned char* value = (unsigned char*) &s[start]; // reinterpret cast
        Label label = {value, end - start};
        Node* node = get_leaf_node_from_label(tree, &label);
        encode_label(bit_stream, tree, node, &label);
        adjust_tree(tree, node, &label);
        i++;
    }
    free(cuts);
}


#define MAX_BUFFER_SIZE 256
/*
 * Read buffer_size bytes at a time into the buffer. The buffer is then encoded optimally
 * by calling encode_string. This does not guarantee the entire file is encoded optimally,
 * as this would require the method to read the entire file into the buffer to calculate the perfect partition.
 * This is however not possible as that would violate the online requirement.
 * Using a larger MAX_BUFFER_SIZE should in theory improve the encoding, but it slows down the program,
 * increases the memory use and in practice any gain depends on the quality of the should_encode_string function.
 * The method will keep adding nodes to the tree until it either halts or reaches the max_bytes limit.
 * At this point the method will only reuse nodes that were already added and only add nodes when absolutely
 * necessary (single byte labels that have not been added before).
 */
void encode(FILE* input_file, FILE* output_file, int max_label_length, size_t max_bytes, int allowed_buffer_size) {
    Tree* tree = create_tree();
    BitStream* bit_stream = init_write_bit_stream(output_file);
    int buffer_size = min_(MAX_BUFFER_SIZE, allowed_buffer_size);
    size_t io_buffer_memory = 8 * 1024;
    size_t misc_memory = (8 * 1024) * (buffer_size / 16);
    size_t working_memory = (2 * buffer_size*buffer_size) + io_buffer_memory;
    size_t max_tree_bytes = max_bytes - working_memory - misc_memory;
    char buffer[MAX_BUFFER_SIZE+1];
    buffer[buffer_size] = '\0';
    bool can_allocate_memory = true;
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, buffer_size, input_file)) == buffer_size) {
        encode_string(bit_stream, tree, buffer, buffer_size, max_label_length, can_allocate_memory);
        can_allocate_memory = get_tree_allocated_bytes(tree) < max_tree_bytes;
    }
    if (bytes_read != 0) {
        encode_string(bit_stream, tree, buffer, bytes_read, max_label_length, can_allocate_memory);
    }
    encode_end(bit_stream, tree);
    free_tree(tree);
    free(bit_stream);
}