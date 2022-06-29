//
// Created by Danila Belous on 28.06.2022 at 19:34.
//

#include "output_wrapper.h"
output_wrapper::output_wrapper(std::ostream& output) : output(output), cc{0, 0} {}
output_wrapper::~output_wrapper() {
    flush();
}

output_wrapper& output_wrapper::operator<<(const cool_char& next) {
  constexpr auto WORD_WIDTH = cool_char::WORD_WIDTH;
  cc.data |= next.data >> cc.nbits;
  if (cc.nbits + next.nbits > WORD_WIDTH) {
    output << static_cast<char>(cc.data);
    cc.data = next.data << (WORD_WIDTH - cc.nbits);
    //    nbits = next.nbits - (WORD_WIDTH - nbits); <=>
    //    nbits += next.nbits - WORD_WIDTH; <=>
    cc.nbits -= WORD_WIDTH - next.nbits;
  } else {
    cc.nbits += next.nbits;
  }
  return *this;
}

output_wrapper& output_wrapper::flush() {
  if (cc.nbits > 0) {
    output << cc.data;
    cc.data = 0;
    cc.nbits = 0;
  }
  return *this;
}
