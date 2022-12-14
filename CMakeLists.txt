cmake_minimum_required(VERSION 3.21)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(huffman)

add_executable(tests integration-tests/tests.cpp)
add_executable(huffman-tool huffman_tool.cpp)

if (NOT MSVC)
    target_compile_options(tests PRIVATE -Wall -Wno-sign-compare -pedantic)
    target_compile_options(huffman-tool PRIVATE -Wall -Wno-sign-compare -pedantic)
endif ()

option(USE_SANITIZERS "Enable to build with undefined, leak and address sanitizers" OFF)
if (USE_SANITIZERS)
    target_compile_options(tests PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
    target_compile_options(huffman-tool PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
    target_link_options(tests PUBLIC -fsanitize=address,undefined,leak)
    target_link_options(huffman-tool PUBLIC -fsanitize=address,undefined,leak)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(tests PUBLIC -stdlib=libc++)
    target_compile_options(huffman-tool PUBLIC -stdlib=libc++)
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(tests PUBLIC -D_GLIBCXX_DEBUG)
    target_compile_options(huffman-tool PUBLIC -D_GLIBCXX_DEBUG)
endif ()

find_package(GTest REQUIRED)
find_package(args REQUIRED)

target_link_libraries(tests PUBLIC huffman GTest::gtest GTest::gtest_main)
target_link_libraries(huffman-tool PUBLIC huffman taywee::args)

target_include_directories(huffman-tool PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/huffman")
