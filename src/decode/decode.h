#ifndef PROJECT_DECODE_H
#define PROJECT_DECODE_H

#include <stdio.h>

/**
 * Decodes data that has been encoded with the encode method.
 * @param input_file The source of the data, decode will read the data from here.
 * @param output_file The destination of the data, decode will write the result to here.
 */
void decode(FILE* input_file, FILE* output_file);

#endif //PROJECT_DECODE_H
