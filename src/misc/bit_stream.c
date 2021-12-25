#include "bit_stream.h"
#include "util.h"


/*
 * current_byte is the byte that's currently being read from/written to.
 * next_bit_index is the index of the next bit that will be read/written in current_byte.
 * file is the file that's being read from/written to.
 */
typedef struct BitStream {
    unsigned char current_byte;
    int next_bit_index;
    FILE* file;
} BitStream;


BitStream* init_read_bit_stream(FILE* input_file) {
    BitStream* bit_stream = safe_malloc(sizeof(BitStream));
    bit_stream->current_byte = fgetc(input_file);
    bit_stream->next_bit_index = 7;
    bit_stream->file = input_file;
    return bit_stream;
}


BitStream* init_write_bit_stream(FILE* output_file) {
    BitStream* bit_stream = safe_malloc(sizeof(BitStream));
    bit_stream->current_byte = 0;
    bit_stream->next_bit_index = 7;
    bit_stream->file = output_file;
    return bit_stream;
}


/*
 * Write the given bit into the position indicated by next_bit_index in current_byte.
 * If we've filled the byte, write it to the file and reset next_bit_index.
 */
void write_bit(BitStream* bit_stream, bool bit) {
    bit_stream->current_byte = set_bit(bit_stream->current_byte, bit, bit_stream->next_bit_index);
    bit_stream->next_bit_index--;
    if (bit_stream->next_bit_index == -1) {
        fputc(bit_stream->current_byte, bit_stream->file);
        bit_stream->next_bit_index = 7;
        bit_stream->current_byte = 0;
    }
}


void write_byte(BitStream* bit_stream, unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        write_bit(bit_stream, get_bit(byte, i));
    }
}


void pad_byte_with_zero_bits(BitStream* bit_stream) {
    while (bit_stream->next_bit_index != 7) {
        write_bit(bit_stream, 0);
    }
}


/*
 * Get the next bit as indicated by next_bit_index from current_byte.
 * If we've fully read the current byte, read the next byte into the bitstream and reset next_bit_index.
 */
bool read_bit(BitStream* bit_stream) {
    bool result = get_bit(bit_stream->current_byte, bit_stream->next_bit_index);
    bit_stream->next_bit_index--;
    if (bit_stream->next_bit_index == -1) {
        bit_stream->current_byte = fgetc(bit_stream->file);
        bit_stream->next_bit_index = 7;
    }
    return result;
}


/*
 * Read 8 bits from the bitstream and form the byte using bit shifts.
 */
unsigned char read_byte(BitStream* bit_stream) {
    unsigned char ch = 0;
    for (int i = 7; i >= 0; i--) {
        bool bit = read_bit(bit_stream);
        ch += bit << i;
    }
    return ch;
}
