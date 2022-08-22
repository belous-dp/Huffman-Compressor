//
// Created by Danila Belous on 28.06.2022 at 12:43.
//

#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include "bit_sequence.h"
#include "output_wrapper.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <limits>
#include <vector>

struct encoder {
  friend struct decoder;
  using word = bit_sequence::word;

  encoder();
  ~encoder();

  void process_input(std::istream& input);

  void print_metadata(std::ostream& output) const;

  void print_codes(std::ostream& output); // todo debug, remove

  void encode(std::istream& input, std::ostream& output) const;

private:
  void collect_statistics(std::istream& input);
  void collect_statistics(word character);
  void build_tree();

  std::array<bit_sequence, bit_sequence::WORD_MAX_VAL + 1> codes;
  std::array<size_t, bit_sequence::WORD_MAX_VAL + 1> frequency;

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

  static void destroy_tree(node* root);
  static bool is_leaf(node* pNode);

  void build_codes(node* root, bit_sequence& cur);
  void print_tree(node* root, output_wrapper& out) const;

};

#endif // HUFFMAN_ENCODER_H
