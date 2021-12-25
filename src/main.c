#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../tests/test.h"
#include "encode/encode.h"
#include "decode/decode.h"


typedef struct Args {
    bool encode_flag;
    bool decode_flag;
    int max_label_length;
    int max_memory_usage;
    bool run_tests_flag;
} Args;


void parse_args(Args* args, int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (strcmp(arg, "-c") == 0) args->encode_flag = true;
        else if (strcmp(arg, "-d") == 0) args->decode_flag = true;
        else if (strcmp(arg, "-t") == 0) args->run_tests_flag = true;
        else if (isdigit(arg[0]) && arg[1] == ',' && isdigit(arg[2])) {
            args->max_label_length = arg[0] - '0';
            if (args->max_label_length == 0) {
                fprintf(stderr, "LEN must be at least 1.\n");
                exit(1);
            }
            args->max_memory_usage = arg[2] - '0';
        } else {
            fprintf(stderr, "'%s' is not a valid argument.\n", arg);
            exit(1);
        }
    }

    if (!args->encode_flag && !args->decode_flag) {
        fprintf(stderr, "Neither the -c nor the -d flag was passed.\n");
        exit(1);
    } else if (args->encode_flag && args->decode_flag) {
        fprintf(stderr, "Both the -c and -d flags were passed.\n");
        exit(1);
    } else if (args->encode_flag && args->max_memory_usage == -1 && args->max_label_length == -1) {
        fprintf(stderr, "The -c flag was passed, but the 'LEN,MEM' argument was not.\n");
        exit(1);
    } else if (args->decode_flag && args->max_memory_usage != -1 && args->max_label_length != -1) {
        fprintf(stderr, "Both the 'LEN,MEM' argument and the -d flag were passed.\n");
        exit(1);
    }
}


#define KIB 1024
#define MIB (1024 * 1024)
#define GIB (1024 * 1024 * 1024)
int main(int argc, char* argv[]) {
    Args args = {false, false, -1, -1, false};
    parse_args(&args, argc, argv);
    if (args.run_tests_flag) {
        run_tests();
    }

#ifdef _WIN32
    #include <io.h>
    #include <fcntl.h>
    // Prevent Windows from translating '\n' bytes to '\r\n'
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    if (args.encode_flag) {
        size_t max_bytes_table[] = {75*KIB, 75*KIB, 100*KIB, 200*KIB, MIB, 20*MIB, 50*MIB, 100*MIB, 500*MIB, GIB};
        int allowed_buffer_size_table[] = {16, 16, 16, 32, 128, 256, 256, 256, 256, 256};
        encode(stdin, stdout, args.max_label_length, max_bytes_table[args.max_memory_usage],
               allowed_buffer_size_table[args.max_memory_usage]);
    } else {
        decode(stdin, stdout);
    }
}