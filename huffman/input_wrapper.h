//
// Created by Danila Belous on 29.06.2022 at 12:11.
//

#ifndef HUFFMAN_INPUT_WRAPPER_H
#define HUFFMAN_INPUT_WRAPPER_H

#include "bit_sequence.h"
#include <istream>
struct input_wrapper {
  explicit input_wrapper(std::istream& input, size_t chunk_size, uint8_t nlb);

  bool has(size_t n);
  uint8_t scan_bit();
  bit_sequence::word scan_word();

private:
  bit_sequence buf;
  std::istream& input;
  size_t chunk_size;
  size_t pos;
  uint8_t nlb;
  void fetch(size_t n);
  bool eof();
};

#endif // HUFFMAN_INPUT_WRAPPER_H
