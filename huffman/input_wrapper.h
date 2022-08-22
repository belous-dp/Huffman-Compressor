//
// Created by Danila Belous on 29.06.2022 at 12:11.
//

#ifndef HUFFMAN_INPUT_WRAPPER_H
#define HUFFMAN_INPUT_WRAPPER_H

#include "bit_sequence.h"
#include <istream>

struct input_wrapper {
  using word = bit_sequence::word;

  explicit input_wrapper(std::istream& input);
  input_wrapper(std::istream& input, uint8_t unused);

  bool has(size_t n = 1);
  uint8_t scan_bit();
  word scan_word();

private:
  static const size_t CHUNK_SIZE = 8192;

  bit_sequence buf;
  std::istream& input;
  size_t pos;
  uint8_t unused;
  void fetch();
  bool eof();
  size_t buf_size() const;
};

#endif // HUFFMAN_INPUT_WRAPPER_H
