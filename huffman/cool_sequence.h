//
// Created by Danila Belous on 30.06.2022 at 08:46.
//

#ifndef HUFFMAN_COOL_SEQUENCE_H
#define HUFFMAN_COOL_SEQUENCE_H

#include "cool_char.h"
#include <vector>

struct cool_sequence {
  cool_sequence();
  cool_sequence(cool_sequence const& other);
  cool_sequence& operator=(cool_sequence const& other);
  void swap(cool_sequence& other);
  cool_sequence& add_bit(uint8_t bit);
  cool_sequence& remove_bit();
  size_t get_nbits();
  friend bool operator ==(cool_sequence const& a, cool_sequence const &b);
  friend bool operator !=(cool_sequence const& a, cool_sequence const &b);
private:
  friend struct output_wrapper;
  std::vector<cool_char::word> seq;
  cool_char last;
};

#endif // HUFFMAN_COOL_SEQUENCE_H
