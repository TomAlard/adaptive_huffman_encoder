#ifndef PROJECT_BIT_STREAM_H
#define PROJECT_BIT_STREAM_H

#include <stdio.h>
#include <stdbool.h>


// Abstracts reading and writing single bits from/to a file.
typedef struct BitStream BitStream;

/*
 * Returns a BitStream in 'read mode', a BitStream initialised in this way
 * can be used with the read_bit and read_byte methods.
 */
BitStream* init_read_bit_stream(FILE* input_file);

/*
 * Returns a BitStream in 'write mode', a BitStream initialised in this way
 * can be used with the write_bit, write_byte and pad_byte_with_zero_bits methods.
 */
BitStream* init_write_bit_stream(FILE* output_file);

// Write a single bit to the bitstream.
void write_bit(BitStream* bit_stream, bool bit);

// Convenience method to write 8 bits (a byte) to the bitstream.
void write_byte(BitStream* bit_stream, unsigned char byte);

// Pad the current byte with zero bits until it is full.
void pad_byte_with_zero_bits(BitStream* bit_stream);

// Read a single bit from the bitstream.
bool read_bit(BitStream* bit_stream);

// Convenience method to read 8 bits (a byte) from the bitstream.
unsigned char read_byte(BitStream* bit_stream);

#endif //PROJECT_BIT_STREAM_H
