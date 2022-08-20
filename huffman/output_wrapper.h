//
// Created by Danila Belous on 28.06.2022 at 19:34.
//

#ifndef HUFFMAN_OUTPUT_WRAPPER_H
#define HUFFMAN_OUTPUT_WRAPPER_H

#include "bit_sequence.h"
#include <climits>
#include <ostream>
struct output_wrapper {
  using word = bit_sequence::word;

  explicit output_wrapper(std::ostream& output);
  ~output_wrapper();

  output_wrapper& print_bit(uint8_t bit);
  output_wrapper& print_word(word w);
  output_wrapper& print_bit_sequence(bit_sequence const& bs);

private:
  bit_sequence buf;
  std::ostream& output;

  output_wrapper& maybe_print();
  output_wrapper& flush();
};

#endif // HUFFMAN_OUTPUT_WRAPPER_H
