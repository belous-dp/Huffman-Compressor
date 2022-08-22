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
    } catch (std::invalid_argument& e) {
      throw std::invalid_argument("metadata corrupted: invalid tree");
    }
    try {
      unused = iw.scan_bit();
      unused |= iw.scan_bit() << 1;
      unused |= iw.scan_bit() << 2;
    } catch (std::invalid_argument& e) {
      throw std::invalid_argument("metadata corrupted: wrong number of unused bits");
    }
  }
}

void decoder::build_tree_dfs(encoder::node*& root, input_wrapper& iw) {
  if (iw) {
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
}

void decoder::decode(std::istream& input, std::ostream& output) const {
  auto iw = input_wrapper(input, unused);
  while (iw) {
    if (encoder::is_leaf(tree)) { // "aaa", "bbb", etc.
      if (iw.scan_bit() != 0) {
        throw std::invalid_argument("invalid data");
      }
    }
    decode_dfs(tree, iw, output);
  }
}

void decoder::decode_dfs(encoder::node* root, input_wrapper& iw,
                         std::ostream& output) const {
  if (encoder::is_leaf(root)) {
    output << static_cast<char>(root->chr);
    return;
  } else {
    if (!iw) {
      throw std::invalid_argument("invalid data");
    }
    if (iw.scan_bit() > 0) {
      decode_dfs(root->right, iw, output);
    } else {
      decode_dfs(root->left, iw, output);
    }
  }
}

void decoder::print_tree(encoder::node* root, output_wrapper& out) const {
  // идея: на внутренних вершинах выводим 0, на листьях -- 1 и код символа
  if (root) {
    if (!encoder::is_leaf(root)) {
      out.print_bit(0);
      print_tree(root->left, out);
      print_tree(root->right, out);
    } else {
      out.print_bit(1).print_word(root->chr);
    }
  }
}
