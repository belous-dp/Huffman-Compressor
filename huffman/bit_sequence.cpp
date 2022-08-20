//
// Created by Danila Belous on 30.06.2022 at 10:23.
//

#include "bit_sequence.h"
#include <cassert>
bit_sequence::bit_sequence() : nlb(0) {}
bit_sequence::bit_sequence(const bit_sequence& other) = default;
bit_sequence& bit_sequence::operator=(const bit_sequence& other) {
  if (this != &other) {
    bit_sequence copy = other;
    swap(copy);
  }
  return *this;
}

void bit_sequence::swap(bit_sequence& other) {
  std::swap(bits, other.bits);
  std::swap(nlb, other.nlb);
}

bit_sequence::~bit_sequence() = default;

uint8_t bit_sequence::bit_at(size_t index) const {
  assert(index < size());
  return static_cast<word>(bits[index / WORD_WIDTH] >>
                           (WORD_WIDTH - index % WORD_WIDTH - 1U)) & 1U;
}

bit_sequence::word bit_sequence::word_at(size_t index) const {
  assert(index < size());
  word res = bits[index / WORD_WIDTH] << (index % WORD_WIDTH);
  if (index / WORD_WIDTH + 1 < bits.size() && index % WORD_WIDTH > 0) {
    res |= static_cast<word>(bits[index / WORD_WIDTH + 1] >>
                             (index % WORD_WIDTH));
  }
  return res;
}

void bit_sequence::append_bit(uint8_t bit) {
  if (bits.empty() || nlb == WORD_WIDTH) {
    bits.push_back(bit << (WORD_WIDTH - 1U));
    nlb = 1;
  } else {
    bits.back() |= static_cast<word>(bit << (WORD_WIDTH - nlb - 1U));
    nlb++;
  }
}

void bit_sequence::append_word(bit_sequence::word w) {
  if (bits.empty() || nlb == WORD_WIDTH) {
    bits.push_back(w);
    nlb = WORD_WIDTH;
  } else {
    bits.back() |= static_cast<word>(w >> nlb);
    w <<= static_cast<uint8_t>(WORD_WIDTH - nlb);
    bits.push_back(w);
  }
}

uint8_t bit_sequence::pop_back_bit() {
  assert(!bits.empty());
  uint8_t bit = static_cast<uint8_t>(bits.back() >>
                                     static_cast<uint8_t>(WORD_WIDTH - nlb)) &
                1U;
  bits.back() ^=
      static_cast<uint8_t>(bit << static_cast<uint8_t>(WORD_WIDTH - nlb));
  nlb--;
  if (nlb == 0) {
    bits.pop_back();
    if (!bits.empty()) {
      nlb = WORD_WIDTH;
    }
  }
  return bit;
}

bool operator==(const bit_sequence& a, const bit_sequence& b) {
  return a.bits == b.bits && a.nlb == b.nlb;
}
bool operator!=(const bit_sequence& a, const bit_sequence& b) {
  return !(a == b);
}
size_t bit_sequence::size() const {
  return (bits.empty() ? 0 : bits.size() - 1) * WORD_WIDTH + nlb;
}
