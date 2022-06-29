//
// Created by Danila Belous on 28.06.2022 at 12:43.
//

#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include "cool_char.h"
#include "output_wrapper.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <limits>
struct encoder {
  friend struct decoder;
  using word = cool_char::word;

  encoder();
  ~encoder();

  void process_input(std::istream& input);
  void process_input(word character);

  void build_tree();
  void print_tree(std::ostream& output);
  void print_codes(std::ostream& output);

  void encode(std::istream& input, std::ostream& output);

private:
  static constexpr word WORD_MAX_VAL = std::numeric_limits<uint8_t>::max();
  std::array<size_t, WORD_MAX_VAL> frequency;
  std::array<cool_char, WORD_MAX_VAL> codes;

  struct node {
    node* left;
    node* right;
    word chr;
    size_t frq;
    node(node* left, node* right);
    node(word chr, size_t frq);
    explicit node(word chr);
  };
  node* tree;
  friend void destroy_tree(node* root);
  friend void build_codes(node* root, cool_char cur,
                   std::array<cool_char, WORD_MAX_VAL>& codes);
  void print_tree_dfs(node* root, output_wrapper& out);
};

#endif // HUFFMAN_ENCODER_H
