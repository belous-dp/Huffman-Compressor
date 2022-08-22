//
// Created by Danila Belous on 29.06.2022 at 12:11.
//

#include "input_wrapper.h"
#include <cassert>
#include <iostream>

using bs = bit_sequence;

input_wrapper::input_wrapper(std::istream& input, uint8_t unused)
    : buf(0), input(input), len(0), unused(unused) {
  assert(unused < bs::WORD_WIDTH);
}

input_wrapper::~input_wrapper() = default;

bool input_wrapper::fetch() {
  if (!input) {
    return false;
  }
  buf = bs::reverse_word(input.get());
  len = bs::WORD_WIDTH;
  if (!input) {
    return false;
  }
  if (eof()) {
    buf &= (1 << (bs::WORD_WIDTH - unused)) - 1;
    len -= unused;
  }
  return true;
}

bool input_wrapper::eof() {
  return input.peek() == std::char_traits<char>::eof();
}

void error() {
  throw std::invalid_argument("bad input request");
}

uint8_t input_wrapper::scan_bit() {
  if (len == 0 && !fetch()) {
    error();
  }
  uint8_t bit = buf & 1;
  buf >>= 1;
  len--;
  return bit;
}

bit_sequence::word input_wrapper::scan_word() {
  if (len == 0 && !fetch()) {
    error();
  }
  bs::word word = buf;
  size_t past_len = len;
  size_t left = bs::WORD_WIDTH - len;
  buf = 0;
  len = 0;
  if (left > 0 && (!fetch() || len < left)) {
    error();
  }
  word |= buf << past_len;
  buf >>= left;
  len -= left;
  return word;
}

input_wrapper::operator bool() {
  return len > 0 || fetch();
}
