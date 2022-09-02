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
  output_wrapper& print_bit_sequence(bit_sequence const& bitseq);

private:

  word buf;
  std::ostream& output;
  size_t len;

  void print(word w);
};

#endif // HUFFMAN_OUTPUT_WRAPPER_H
