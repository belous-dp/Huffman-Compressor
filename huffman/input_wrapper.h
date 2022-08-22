//
// Created by Danila Belous on 29.06.2022 at 12:11.
//

#ifndef HUFFMAN_INPUT_WRAPPER_H
#define HUFFMAN_INPUT_WRAPPER_H

#include "bit_sequence.h"
#include <istream>

struct input_wrapper {
  explicit input_wrapper(std::istream& input, uint8_t unused);
  ~input_wrapper();

  uint8_t scan_bit();
  bit_sequence::word scan_word();

  explicit operator bool();

private:
  bit_sequence::word buf;
  std::istream& input;
  size_t len;
  uint8_t unused;
  bool fetch();
  bool eof();
};

#endif // HUFFMAN_INPUT_WRAPPER_H
