//
// Created by Danila Belous on 30.06.2022 at 10:23.
//

#include "bit_sequence.h"
#include <cassert>
bit_sequence::bit_sequence(size_t reserve_size) : len(0) {
  if (reserve_size > 0) {
    bits.reserve(reserve_size);
  }
}
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
  std::swap(len, other.len);
}

bit_sequence::~bit_sequence() = default;

uint8_t bit_sequence::bit_at(size_t index) const {
  assert(index < len);
  return (bits[index / WORD_WIDTH] >> (index % WORD_WIDTH)) & 1U;
}

bit_sequence::word bit_sequence::word_at(size_t index) const {
  assert(index < len);
  size_t id = index / WORD_WIDTH;
  size_t pos = index % WORD_WIDTH;
  word res = bits[id] >> pos;
  if (pos && id + 1 < bits.size()) {
    res |= bits[id + 1] << (WORD_WIDTH - pos);
  }
  return res;
}

bit_sequence::word bit_sequence::reverse_word(word w) {
  word rw = 0;
  for (size_t i = 0; i < WORD_WIDTH; ++i, w >>= 1) {
    rw = (rw << 1) | (w & 1);
  }
  return rw;
}

void bit_sequence::append_bit(uint8_t bit) {
  assert(bit == 0 || bit == 1);
  if (len % WORD_WIDTH == 0) {
    bits.push_back(bit);
  } else {
    bits.back() |= bit << (len % WORD_WIDTH);
  }
  len++;
}

void bit_sequence::append_word(word w) {
  size_t pos = len % WORD_WIDTH;
  if (pos == 0) {
    bits.push_back(w);
  } else {
    bits.back() |= w << pos;
    bits.push_back(w >> (WORD_WIDTH - pos));
  }
  len += WORD_WIDTH;
}

uint8_t bit_sequence::pop_back_bit() {
  assert(len > 0);
  uint8_t bit = bit_at(len - 1);
  size_t pos = (len - 1) % WORD_WIDTH;
  bits.back() &= (1 << pos) - 1;
  if (pos == 0) {
    bits.pop_back();
  }
  len--;
  return bit;
}

bool operator==(const bit_sequence& a, const bit_sequence& b) {
  return a.len == b.len && a.bits == b.bits;
}
bool operator!=(const bit_sequence& a, const bit_sequence& b) {
  return !(a == b);
}

size_t bit_sequence::size() const {
  return len;
}
