cmake_minimum_required(VERSION 3.21)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

ADD_LIBRARY(Huffman STATIC huffman/cool_char.cpp huffman/decoder.cpp huffman/encoder.cpp huffman/input_wrapper.cpp huffman/output_wrapper.cpp)

add_executable(main huffman_tool.cpp)

if (NOT MSVC)
    target_compile_options(main PRIVATE -Wall -Wno-sign-compare -pedantic)
endif ()

option(USE_SANITIZERS "Enable to build with undefined,leak and address sanitizers" OFF)
if (USE_SANITIZERS)
    target_compile_options(main PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
    target_link_options(main PUBLIC -fsanitize=address,undefined,leak)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(main PUBLIC -stdlib=libc++)
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(main PUBLIC -D_GLIBCXX_DEBUG)
endif ()

find_package(args REQUIRED)
target_link_libraries(main PRIVATE Huffman taywee::args)

