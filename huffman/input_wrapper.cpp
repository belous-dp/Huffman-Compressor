//
// Created by Danila Belous on 29.06.2022 at 12:11.
//

#include "input_wrapper.h"
#include <cassert>
#include <iostream>

input_wrapper::input_wrapper(std::istream& input) : cc({0, 0}), input(input), last_returned(false) {}

input_wrapper& input_wrapper::operator>>(cool_char& res) {
  constexpr auto WORD_WIDTH = cool_char::WORD_WIDTH;
  res.data = cc.data; // добавили то, что есть
  uint8_t delta;
  if (res.nbits > cc.nbits) {
//    assert(input);
//    if (!input) {
//      std::cerr << "incorrect input request" << std::endl;
//      exit(0);
//    }
    cc.data = input.get(); // считали то, чего не хватает + какой-то хвост
    if (!input) {
      std::cerr << "incorrect input request" << std::endl;
      exit(0);
    }
    res.data |= cc.data >> cc.nbits; // добавили то, чего не хватает
    delta = res.nbits - cc.nbits;
    cc.nbits += WORD_WIDTH; // считали
  } else {
    delta = res.nbits;
  }
  res.data >>= (WORD_WIDTH - res.nbits); // занулили хвост, шаг 1
  res.data <<= (WORD_WIDTH - res.nbits); // занулили хвост, шаг 2
  cc.data <<= delta; // удалили из начала использованные биты
  cc.nbits -= res.nbits; // уменьшили на количество использованных
  last_returned = res.nbits > 0;
  return *this;
}

input_wrapper::operator bool() const {
  return input.operator bool() || cc.nbits > 0 || last_returned;
}

bool input_wrapper::eof() {
  return input.peek() == std::char_traits<char>::eof();
}
