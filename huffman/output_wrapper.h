//
// Created by Danila Belous on 28.06.2022 at 19:34.
//

#ifndef HUFFMAN_OUTPUT_WRAPPER_H
#define HUFFMAN_OUTPUT_WRAPPER_H

#include "cool_char.h"
#include "cool_sequence.h"
#include <climits>
#include <ostream>
struct output_wrapper {

//  word data; // биты хранятся в старших разрядах
//  word nbits;

  explicit output_wrapper(std::ostream& output);
  ~output_wrapper();

  output_wrapper& operator <<(cool_char const& next);
  output_wrapper& operator <<(cool_sequence const& next);

private:
  std::ostream& output;
  cool_char cc;
  output_wrapper& flush();
};

#endif // HUFFMAN_OUTPUT_WRAPPER_H
