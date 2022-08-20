//
// Created by Danila Belous on 30.06.2022 at 10:23.
//

#ifndef HUFFMAN_BIT_SEQUENCE_H
#define HUFFMAN_BIT_SEQUENCE_H

#include <climits>
#include <cstdint>
#include <limits>
#include <vector>
struct bit_sequence {
  using word = uint8_t;
  static constexpr word WORD_MAX_VAL = std::numeric_limits<word>::max();
  static constexpr uint8_t WORD_WIDTH = sizeof(word) * CHAR_BIT;
  bit_sequence();
  bit_sequence(bit_sequence const& other);
  bit_sequence& operator=(bit_sequence const& other);
  void swap(bit_sequence& other);
  ~bit_sequence();
  uint8_t bit_at(size_t index) const;
  word word_at(size_t index) const;
  void append_bit(uint8_t bit);
  void append_word(word w);
  uint8_t pop_back_bit();
  friend bool operator==(bit_sequence const& a, bit_sequence const& b);
  friend bool operator!=(bit_sequence const& a, bit_sequence const& b);
  size_t size() const;

protected:
  std::vector<uint8_t> bits;
  uint8_t nlb; // number of useful bits in last byte
};

#endif // HUFFMAN_BIT_SEQUENCE_H
