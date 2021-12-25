#ifndef PROJECT_ENCODE_H
#define PROJECT_ENCODE_H

#include "../tree/tree.h"

/**
 * Encodes arbitrary data using a modified version of Adaptive Huffman.
 * @param input_file The source of the data, encode will read the data from here.
 * @param output_file The destination of the encoded data, encode will write the result to here.
 * @param max_label_length The maximum length of a label in the Huffman Tree. Allowing the method to use larger
 *                         labels should theoretically increase the compression factor, but slow down the program
 *                         and increase the memory use.
 * @param max_bytes The maximum amount of bytes the method is allowed to use. Allowing the method to use more
 *                  bytes should theoretically increase the compression factor.
 * @param allowed_buffer_size The maximum amount of bytes that may be read and partitioned in one go.
 */
void encode(FILE* input_file, FILE* output_file, int max_label_length, size_t max_bytes, int allowed_buffer_size);

#endif //PROJECT_ENCODE_H
