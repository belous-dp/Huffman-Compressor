//
// Created by Danila Belous on 28.06.2022 at 23:02.
//

#include "cool_char.h"

cool_char::cool_char() : cool_char(0, 0) {}

cool_char::cool_char(cool_char::word bit) : cool_char(bit, 1) {}

cool_char::cool_char(cool_char::word data, uint8_t nbits)
    : data(data << (WORD_WIDTH - nbits)), nbits(nbits) {}

cool_char cool_char::add_bit(uint8_t bit) const {
  cool_char res;
  res.data = static_cast<word>(
      data | static_cast<uint8_t>(bit << (WORD_WIDTH - nbits - 1U)));
  res.nbits = static_cast<uint8_t>(nbits + 1);
  return res;
}
