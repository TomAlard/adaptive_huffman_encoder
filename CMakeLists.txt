cmake_minimum_required(VERSION 3.17)
project(project C)

set(CMAKE_C_STANDARD 99)

add_executable(project src/main.c src/encode/encode.c src/encode/encode.h src/misc/util.c src/misc/util.h tests/test.c tests/test.h src/tree/tree.c src/tree/tree.h src/decode/decode.c src/decode/decode.h src/tree/node_array_list.c src/tree/node_array_list.h src/misc/bit_stream.c src/misc/bit_stream.h src/encode/encode_strategy.c src/encode/encode_strategy.h src/tree/node.c src/tree/node.h src/encode/minimum_cuts.c src/encode/minimum_cuts.h src/tree/internal_trie.c src/tree/internal_trie.h src/encode/trie.c src/encode/trie.h)