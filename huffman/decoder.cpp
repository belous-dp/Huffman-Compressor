//
// Created by Danila Belous on 29.06.2022 at 00:26.
//

#include "decoder.h"
#include "input_wrapper.h"
#include <cstddef>
#include <iostream>

using bs = bit_sequence;

decoder::decoder() : tree(nullptr), unused(0) {}

decoder::~decoder() {
  destroy_tree(tree);
}

void decoder::scan_metadata(std::istream& input) {
  auto iw = input_wrapper(input);
  if (iw.has()) {
    build_tree_dfs(tree, iw);
    if (!iw.has(3)) {
      throw std::invalid_argument("metadata corrupted");
    }
    unused = iw.scan_bit();
    unused |= iw.scan_bit() << 1;
    unused |= iw.scan_bit() << 2;
  }
}

void decoder::build_tree_dfs(encoder::node*& root, input_wrapper& iw) {
  if (iw.has()) {
    uint8_t bit = iw.scan_bit();
    if (bit > 0) {
      if (!iw.has(bs::WORD_WIDTH)) {
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
  auto iw = input_wrapper(input, unused);
  while (iw.has()) {
    decode_dfs(tree, iw, output);
  }
}

void decoder::decode_dfs(encoder::node* root, input_wrapper& iw,
                         std::ostream& output) const {
  if (is_leaf(root)) {
    output << static_cast<char>(root->chr);
    return;
  } else {
    if (!iw.has()) {
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
