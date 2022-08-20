//
// Created by Danila Belous on 29.06.2022 at 00:26.
//

#include "decoder.h"
#include "input_wrapper.h"
#include <cstddef>
#include <iostream>

decoder::decoder() : tree(nullptr), nlast_bits(0) {}

decoder::~decoder() {
  destroy_tree(tree);
}

void decoder::scan_metadata(std::istream& input) {
  auto iw =
      input_wrapper(input, bit_sequence::WORD_WIDTH, bit_sequence::WORD_WIDTH);
  if (iw.has(1)) {
    build_tree_dfs(tree, iw);
    if (!iw.has(bit_sequence::WORD_WIDTH)) {
      throw std::invalid_argument("metadata corrupted");
    }
    nlast_bits = iw.scan_word();
  }
}

void decoder::build_tree_dfs(encoder::node*& root, input_wrapper& iw) {
  if (iw.has(1)) {
    uint8_t bit = iw.scan_bit();
    if (bit > 0) {
      if (!iw.has(bit_sequence::WORD_WIDTH)) {
        throw std::invalid_argument("invalid tree");
      }
      root = new encoder::node(iw.scan_word());
    } else {
      root = new encoder::node(nullptr, nullptr);
      build_tree_dfs(root->left, iw);
      build_tree_dfs(root->right, iw);
      if (!root->right) {
        throw std::invalid_argument("invalid tree");
      }
    }
  }
}

void decoder::decode(std::istream& input, std::ostream& output) const {
  auto iw = input_wrapper(
      input, static_cast<size_t>(bit_sequence::WORD_WIDTH * 4096U), nlast_bits);
  while (iw.has(1)) {
    decode_dfs(tree, iw, output);
  }
}

void decoder::decode_dfs(encoder::node* root, input_wrapper& iw,
                         std::ostream& output) const {
  if (!root->left) { // leaf
    output << static_cast<char>(root->chr);
    return;
  } else {
    if (!iw.has(1)) {
      throw std::invalid_argument("invalid data");
      //      output.flush();
      //      std::cerr << "invalid data" << std::endl;
      //      exit(1);
    }
    if (iw.scan_bit() > 0) {
      decode_dfs(root->right, iw, output);
    } else {
      decode_dfs(root->left, iw, output);
    }
  }
}
