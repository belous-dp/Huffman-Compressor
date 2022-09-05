//
// Created by Danila Belous on 29.06.2022 at 00:26.
//

#include "decoder.h"
#include "input_wrapper.h"
#include <iostream>

using bs = bit_sequence;

decoder::decoder() : tree(nullptr), unused(0) {}

decoder::~decoder() {
  encoder::destroy_tree(tree);
}

void decoder::process_metadata(std::istream& input) {
  auto iw = input_wrapper(input, 0);
  if (iw) {
    try {
      build_tree_dfs(tree, iw);
    } catch (std::ios_base::failure& e) {
      throw std::runtime_error("metadata corrupted: invalid tree");
    }
    try {
      unused = iw.scan_bit();
      unused |= iw.scan_bit() << 1;
      unused |= iw.scan_bit() << 2;
    } catch (std::ios_base::failure& e) {
      throw std::runtime_error("metadata corrupted: wrong number of unused bits");
    }
  }
}

void decoder::build_tree_dfs(encoder::node*& root, input_wrapper& iw) {
  if (iw.scan_bit() > 0) {
    root = new encoder::node(iw.scan_word());
  } else {
    root = new encoder::node(nullptr, nullptr);
    build_tree_dfs(root->left, iw);
    build_tree_dfs(root->right, iw);
    if (encoder::is_leaf(root)) {
      throw std::invalid_argument("invalid tree");
    }
  }
}

void decoder::decode(std::istream& input, std::ostream& output) const {
  auto iw = input_wrapper(input, unused);
  if (tree && encoder::is_leaf(tree)) { // "aaa", "bbb", etc.
    while (iw) {
      if (iw.scan_bit() != 0) {
        throw std::invalid_argument("invalid data");
      }
      output << static_cast<char>(tree->chr);
    }
  }
  while (iw) {
    encoder::node* v = tree;
    while (!encoder::is_leaf((v))) {
      v = iw.scan_bit() > 0 ? v->right : v->left;
    }
    output << static_cast<char>(v->chr);
  }
}
