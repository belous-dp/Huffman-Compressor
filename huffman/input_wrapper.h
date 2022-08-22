//
// Created by Danila Belous on 29.06.2022 at 12:11.
//

#ifndef HUFFMAN_INPUT_WRAPPER_H
#define HUFFMAN_INPUT_WRAPPER_H

#include "bit_sequence.h"
#include <istream>

struct input_wrapper {
  using word = bit_sequence::word;

  explicit input_wrapper(std::istream& input, uint8_t unused,
                         size_t fetch_size = 8192);

  bool has(size_t n = 1);
  uint8_t scan_bit();
  word scan_word();

private:
  bit_sequence buf;
  std::istream& input;
  size_t fetch_size;
  size_t pos;
  uint8_t unused;
  void fetch(size_t n);
  bool eof();
  size_t buf_size() const;
};

#endif // HUFFMAN_INPUT_WRAPPER_H
