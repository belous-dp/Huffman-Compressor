//
// Created by Danila Belous on 28.06.2022 at 12:43.
//

#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include "cool_char.h"
#include "cool_sequence.h"
#include "output_wrapper.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <limits>
#include <vector>

struct encoder {
  friend struct decoder;
  using word = cool_char::word;

  encoder();
  ~encoder();

  void process_input(std::istream& input);
  void process_input(word character);

  void build_tree();
  void print_metadata(std::ostream& output);

  std::vector<cool_sequence> get_codes();

  void encode(std::istream& input, std::ostream& output);

private:
  std::array<cool_sequence, cool_char::WORD_MAX_VAL> codes;
  std::array<size_t, cool_char::WORD_MAX_VAL> frequency;

  void print_codes(std::ostream& output);

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
  void build_codes(node* root, cool_sequence& cur);
  void print_tree_dfs(node* root, output_wrapper& out);
};

#endif // HUFFMAN_ENCODER_H
