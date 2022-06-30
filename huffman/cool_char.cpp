//
// Created by Danila Belous on 28.06.2022 at 23:02.
//

#include "cool_char.h"
#include <cassert>

cool_char::cool_char() : cool_char(0, 0) {}

cool_char::cool_char(cool_char::word bit) : cool_char(bit, 1) {}

cool_char::cool_char(cool_char::word data, uint8_t nbits)
    : data(data << (WORD_WIDTH - nbits)), nbits(nbits) {}

cool_char& cool_char::add_bit(uint8_t bit) {
  assert(nbits < WORD_WIDTH);
  data |= static_cast<uint8_t>(bit << (WORD_WIDTH - nbits - 1U));
  nbits++;
  return *this;
}

cool_char& cool_char::remove_bit() {
  assert(nbits > 0);
  uint8_t delta = WORD_WIDTH - nbits;
  uint8_t bit = static_cast<uint8_t>(data >> delta) & 1U;
  data ^= static_cast<uint8_t>(bit << delta);
  nbits--;
  return *this;
}

bool operator==(const cool_char& a, const cool_char& b) {
  return a.data == b.data && a.nbits == b.nbits;
}

bool operator!=(const cool_char& a, const cool_char& b) {
  return !(a == b);
}
