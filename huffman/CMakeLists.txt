set(LIB_NAME huffman)

add_library(${LIB_NAME} STATIC cool_char.cpp encoder.cpp decoder.cpp input_wrapper.cpp output_wrapper.cpp cool_sequence.cpp cool_sequence.h)

if (NOT MSVC)
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wno-sign-compare -pedantic)
endif ()

option(USE_SANITIZERS "Enable to build with undefined, leak and address sanitizers" OFF)
if (USE_SANITIZERS)
    target_compile_options(${LIB_NAME} PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
    target_link_options(${LIB_NAME} PUBLIC -fsanitize=address,undefined,leak)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${LIB_NAME} PUBLIC -stdlib=libc++)
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${LIB_NAME} PUBLIC -D_GLIBCXX_DEBUG)
endif ()

