cmake_minimum_required(VERSION 3.21)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

set(LIB_NAME huffman)
add_library(${LIB_NAME} STATIC huffman/cool_char.cpp
                           huffman/decoder.cpp       huffman/encoder.cpp
                           huffman/input_wrapper.cpp huffman/output_wrapper.cpp)


#set(APP_EXE tests)
set(APP_EXE huffman-tool)

#add_executable(${APP_EXE} unit-tests/tests.cpp)
add_executable(${APP_EXE} huffman_tool.cpp)

if (NOT MSVC)
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wno-sign-compare -pedantic)
    target_compile_options(${APP_EXE} PRIVATE -Wall -Wno-sign-compare -pedantic)
endif ()

option(USE_SANITIZERS "Enable to build with undefined, leak and address sanitizers" OFF)
if (USE_SANITIZERS)
    target_compile_options(${LIB_NAME} PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
    target_compile_options(${APP_EXE} PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
    target_link_options(${LIB_NAME} PUBLIC -fsanitize=address,undefined,leak)
    target_link_options(${APP_EXE} PUBLIC -fsanitize=address,undefined,leak)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${LIB_NAME} PUBLIC -stdlib=libc++)
    target_compile_options(${APP_EXE} PUBLIC -stdlib=libc++)
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${LIB_NAME} PUBLIC -D_GLIBCXX_DEBUG)
    target_compile_options(${APP_EXE} PUBLIC -D_GLIBCXX_DEBUG)
endif ()

#find_package(GTest REQUIRED)
find_package(args REQUIRED)
#target_link_libraries(${APP_EXE} PRIVATE ${LIB_NAME} GTest::gtest GTest::gtest_main)
target_link_libraries(${APP_EXE} PRIVATE ${LIB_NAME} taywee::args)
