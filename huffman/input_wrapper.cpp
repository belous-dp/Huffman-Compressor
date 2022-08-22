//
// Created by Danila Belous on 29.06.2022 at 12:11.
//

#include "input_wrapper.h"
#include <cassert>
#include <iostream>

using bs = bit_sequence;

input_wrapper::input_wrapper(std::istream& input, uint8_t unused,
                             size_t fetch_size)
    : input(input), fetch_size(fetch_size), pos(0), unused(unused) {
  assert(unused < bs::WORD_WIDTH);
}

bool input_wrapper::has(size_t n) {
  if (buf_size() < n) {
    fetch(fetch_size);
  }
  return buf_size() >= n;
}

size_t input_wrapper::buf_size() const {
  return buf.size() - pos;
}

void input_wrapper::fetch(size_t n) {
  if (!input) {
    return;
  }
  bit_sequence nbuf;
  for (size_t i = pos; i + bs::WORD_WIDTH < buf.size(); ++i) {
    nbuf.append_word(buf.word_at(i));
  }
  for (size_t i = buf.size() < bs::WORD_WIDTH
                    ? pos
                    : std::max(buf.size() - bs::WORD_WIDTH, pos);
       i < buf.size(); ++i) {
    nbuf.append_bit(buf.bit_at(i));
  }
  buf = nbuf;
  pos = 0;
  while (buf_size() < n) {
    word w = bs::reverse_word(input.get());
    if (!input) {
      return;
    }
    buf.append_word(w);
    if (eof()) {
      for (size_t i = 0; i < unused; ++i) {
        buf.pop_back_bit();
      }
    }
  }
}

bool input_wrapper::eof() {
  return input.peek() == std::char_traits<char>::eof();
}

uint8_t input_wrapper::scan_bit() {
  if (!has()) {
    throw std::invalid_argument("bad input request");
  }
  return buf.bit_at(pos++);
}

bit_sequence::word input_wrapper::scan_word() {
  if (!has(bs::WORD_WIDTH)) {
    throw std::invalid_argument("bad input request");
  }
  word res = buf.word_at(pos);
  pos += bs::WORD_WIDTH;
  return res;
}
