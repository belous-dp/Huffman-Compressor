//
// Created by Danila Belous on 30.06.2022 at 08:46.
//

#include "cool_sequence.h"
#include <cassert>

cool_sequence& cool_sequence::add_bit(uint8_t bit) {
  if (last.nbits == cool_char::WORD_WIDTH) {
    seq.push_back(last.data);
    last = cool_char();
  }
  last.add_bit(bit);
  return *this;
}

cool_sequence& cool_sequence::remove_bit() {
  if (last.nbits == 0) {
    assert(!seq.empty());
    last = cool_char(seq.back(), cool_char::WORD_WIDTH);
    seq.pop_back();
  }
  last.remove_bit();
  return *this;
}

size_t cool_sequence::get_nbits() {
  return seq.size() * cool_char::WORD_WIDTH + last.nbits;
}

bool operator==(const cool_sequence& a, const cool_sequence& b) {
  if (a.seq != b.seq) {
    return false;
  }
  return a.last == b.last;
}

bool operator!=(const cool_sequence& a, const cool_sequence& b) {
  return !(a == b);
}
cool_sequence::cool_sequence(const cool_sequence& other) : seq(other.seq), last(other.last) {}
cool_sequence& cool_sequence::operator=(const cool_sequence& other) {
  if (this != &other) {
    cool_sequence copy = other;
    swap(copy);
  }
  return *this;
}
void cool_sequence::swap(cool_sequence& other) {
  std::swap(seq, other.seq);
  std::swap(last, other.last);
}
cool_sequence::cool_sequence() : seq(), last() {}
