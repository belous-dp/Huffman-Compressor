//
// Created by Danila Belous on 28.06.2022 at 21:13.
//

#ifndef HUFFMAN_COOL_CHAR_H
#define HUFFMAN_COOL_CHAR_H

#include <climits>
#include <cstdint>
struct cool_char {
  using word = uint8_t;
  static constexpr uint8_t WORD_WIDTH = sizeof(word) * CHAR_BIT;
  word data;
  uint8_t nbits;
  cool_char();
  cool_char(word data);
  cool_char(word data, uint8_t nbits);
  cool_char add_bit(uint8_t bit);
};

#endif // HUFFMAN_COOL_CHAR_H
