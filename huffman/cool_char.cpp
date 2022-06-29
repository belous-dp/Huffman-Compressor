//
// Created by Danila Belous on 28.06.2022 at 23:02.
//

#include "cool_char.h"

cool_char::cool_char() : cool_char(0, 0) {}

cool_char::cool_char(cool_char::word data)
    : cool_char(data << (WORD_WIDTH - 1), 1) {}

cool_char::cool_char(cool_char::word data, uint8_t nbits)
    : data(data), nbits(nbits) {}

cool_char cool_char::add_bit(uint8_t bit) {
  return {static_cast<word>(data | bit << (WORD_WIDTH - nbits - 1)),
          static_cast<uint8_t>(nbits + 1)};
}
