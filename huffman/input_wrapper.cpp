//
// Created by Danila Belous on 29.06.2022 at 12:11.
//

#include "input_wrapper.h"
#include <cassert>
#include <iostream>

input_wrapper::input_wrapper(std::istream& input, size_t chunk_size,
                             uint8_t nlb)
    : input(input), chunk_size(chunk_size), pos(0), nlb(nlb) {
  if (nlb > bit_sequence::WORD_WIDTH) {
    throw std::invalid_argument("number of useful bits in the last word "
                                "cannot be greater that word width");
  }
}

bool input_wrapper::has(size_t n) {
  if (buf.size() - pos < n) {
    fetch(std::max(chunk_size, n));
  }
  return buf.size() - pos >= n;
}

void input_wrapper::fetch(size_t n) {
  if (!input) {
    return;
  }
  bit_sequence nbuf;
  for (size_t i = pos; i + bit_sequence::WORD_WIDTH < buf.size(); ++i) {
    nbuf.append_word(buf.word_at(i));
  }
  for (size_t i = buf.size() < bit_sequence::WORD_WIDTH
                    ? pos
                    : std::max(buf.size() - bit_sequence::WORD_WIDTH, pos);
       i < buf.size(); ++i) {
    nbuf.append_bit(buf.bit_at(i));
  }
  buf = nbuf;
  pos = 0;
  while (buf.size() < n) {
    bit_sequence::word w = input.get();
    if (!input) {
      return;
    }
    buf.append_word(w);
    if (eof()) {
      for (size_t i = 0; i < bit_sequence::WORD_WIDTH - nlb; ++i) {
        buf.pop_back_bit();
      }
      nlb = 0;
    }
  }
}

bool input_wrapper::eof() {
  return input.peek() == std::char_traits<char>::eof();
}

uint8_t input_wrapper::scan_bit() {
  if (!has(1)) {
    throw std::invalid_argument("bad input request");
  }
  return buf.bit_at(pos++);
}

bit_sequence::word input_wrapper::scan_word() {
  if (!has(bit_sequence::WORD_WIDTH)) {
    throw std::invalid_argument("bad input request");
  }
  bit_sequence::word res = buf.word_at(pos);
  pos += bit_sequence::WORD_WIDTH;
  return res;
}
