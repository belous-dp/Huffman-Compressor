//
// Created by Danila Belous on 28.06.2022 at 19:34.
//

#include "output_wrapper.h"

#include <cstddef>

using bs = bit_sequence;

output_wrapper::output_wrapper(std::ostream& output) : buf(0), output(output), len(0) {}

output_wrapper::~output_wrapper() {
  if (len > 0) {
    print(buf);
  }
}

void output_wrapper::print(output_wrapper::word w) {
//  output << static_cast<char>(bs::reverse_word(w));
  output << static_cast<char>(w);
}

output_wrapper& output_wrapper::print_bit(uint8_t bit) {
  buf |= bit << len;
  len++;
  if (len == bs::WORD_WIDTH) {
    print(buf);
    buf = 0;
    len = 0;
  }
  return *this;
}

output_wrapper& output_wrapper::print_word(word w) {
  if (len == 0) {
    print(w);
    return *this;
  }
  size_t left = bs::WORD_WIDTH - len;
  print(buf | (w << len));
  w >>= left;
  buf = w;
  return *this;
}

output_wrapper& output_wrapper::print_bit_sequence(bit_sequence const& bitseq) {
  size_t i = 0;
  if (len > 0) {
    for (; i < bs::WORD_WIDTH - len && i < bitseq.size(); ++i) {
      print_bit(bitseq.bit_at(i));
    }
  }
  for (; i + bs::WORD_WIDTH <= bitseq.size(); i += bs::WORD_WIDTH) {
    print_word(bitseq.word_at(i));
  }
  for (; i < bitseq.size(); ++i) {
    print_bit(bitseq.bit_at(i));
  }
  return *this;
}
