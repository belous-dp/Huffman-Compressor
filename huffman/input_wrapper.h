//
// Created by Danila Belous on 29.06.2022 at 12:11.
//

#ifndef HUFFMAN_INPUT_WRAPPER_H
#define HUFFMAN_INPUT_WRAPPER_H

#include "cool_char.h"
#include <istream>
struct input_wrapper {
  explicit input_wrapper(std::istream& input);

  input_wrapper& operator >>(cool_char& res);
  explicit operator bool() const;
  bool eof();

private:
  std::istream& input;
  cool_char cc;
};

#endif // HUFFMAN_INPUT_WRAPPER_H
