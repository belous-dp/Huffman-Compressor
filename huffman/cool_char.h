//
// Created by Danila Belous on 28.06.2022 at 21:13.
//

#ifndef HUFFMAN_COOL_CHAR_H
#define HUFFMAN_COOL_CHAR_H

#include <climits>
#include <cstdint>
#include <limits>
struct cool_char {
  using word = uint8_t;
  static constexpr word WORD_MAX_VAL = std::numeric_limits<word>::max();
  static constexpr uint8_t WORD_WIDTH = sizeof(word) * CHAR_BIT;
  static constexpr uint8_t WORD_WIDTH_WIDTH = 3;
  word data;
  uint8_t nbits;
  cool_char();
  cool_char(word bit);
  cool_char(word data, uint8_t nbits);
  cool_char add_bit(uint8_t bit) const;
};

#endif // HUFFMAN_COOL_CHAR_H
