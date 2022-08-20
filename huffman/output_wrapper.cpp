//
// Created by Danila Belous on 28.06.2022 at 19:34.
//

#include "output_wrapper.h"

#include <cstddef>

output_wrapper::output_wrapper(std::ostream& output) : output(output) {}

output_wrapper::~output_wrapper() {
  flush();
}

output_wrapper& output_wrapper::flush() {
  for (size_t i = 0; i < buf.size(); i += bit_sequence::WORD_WIDTH) {
    output << static_cast<char>(buf.word_at(i));
  }
  return *this;
}

output_wrapper& output_wrapper::print_bit(uint8_t bit) {
  buf.append_bit(bit);
  return maybe_print();
}

output_wrapper& output_wrapper::print_word(output_wrapper::word w) {
  buf.append_word(w);
  return maybe_print();
}

output_wrapper& output_wrapper::maybe_print() {
  if (buf.size() >
      static_cast<unsigned long>(bit_sequence::WORD_WIDTH * 4096U)) {
    for (size_t i = 0; i + bit_sequence::WORD_WIDTH < buf.size();
         i += bit_sequence::WORD_WIDTH) {
      output << static_cast<char>(buf.word_at(i));
    }
    bit_sequence nbuf;
    for (size_t i = buf.size() < bit_sequence::WORD_WIDTH
                      ? 0
                      : buf.size() - bit_sequence::WORD_WIDTH;
         i < buf.size(); ++i) {
      nbuf.append_bit(buf.bit_at(i));
    }
    buf = nbuf;
  }
  return *this;
}

output_wrapper& output_wrapper::print_bit_sequence(bit_sequence const& bs) {
  for (size_t i = 0; i + bit_sequence::WORD_WIDTH < bs.size(); ++i) {
    print_word(bs.word_at(i));
  }
  for (size_t i = bs.size() < bit_sequence::WORD_WIDTH
                    ? 0
                    : bs.size() - bit_sequence::WORD_WIDTH;
       i < bs.size(); ++i) {
    print_bit(bs.bit_at(i));
  }
  return *this;
}
