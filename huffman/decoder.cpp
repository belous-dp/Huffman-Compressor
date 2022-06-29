//
// Created by Danila Belous on 29.06.2022 at 00:26.
//

#include "decoder.h"
#include "input_wrapper.h"
#include <cassert>
#include <iostream>

decoder::decoder() : codes({}), tree(nullptr) {}

decoder::~decoder() {
  destroy_tree(tree);
}

void decoder::scan_and_build_tree(std::istream& input) {
  auto iw = input_wrapper(input);
  if (iw) {
    scan_and_build_tree_dfs(tree, iw);
  }
  build_codes(tree, {0, 0}, codes); // todo ?
}

void decoder::scan_and_build_tree_dfs(encoder::node*& root, input_wrapper& iw) {
  auto bit = cool_char(0);
  iw >> bit;
  if (iw) {
    if (bit.data > 0) {
      auto word = cool_char(0, cool_char::WORD_WIDTH);
      iw >> word;
      if (!iw) {
        std::cerr << "invalid tree" << std::endl;
        exit(0);
      }
      root = new encoder::node(word.data);
    } else {
      root = new encoder::node(nullptr, nullptr);
      scan_and_build_tree_dfs(root->left, iw);
      scan_and_build_tree_dfs(root->right, iw);
      if (!root->right) {
        std::cerr << "invalid tree" << std::endl;
        exit(0);
      }
    }
  }
}

void decoder::decode(std::istream& input, std::ostream& output) {
  auto iw = input_wrapper(input);
  while (iw) {
    decode_dfs(tree, iw, output);
  }
}

void decoder::print_tree(std::ostream& output) { // todo убрать
  output_wrapper ow = output_wrapper(output);
  print_tree_dfs(tree, ow);
}

void decoder::print_tree_dfs(encoder::node* root, output_wrapper& out) {
  // идея: на внутренних вершинах выводим 0, на листьях -- 1 и код символа
  if (root) {
    if (root->left) {
      assert(root->right);
      out << 0;
      print_tree_dfs(root->left, out);
      print_tree_dfs(root->right, out);
    } else {
      out << 1 << cool_char(root->chr, cool_char::WORD_WIDTH);
    }
  }
}

void decoder::decode_dfs(encoder::node* root, input_wrapper& iw,
                         std::ostream& output) {
  if (!root->left) { // leaf
    output << static_cast<char>(root->chr);
    return;
  } else {
    auto bit = cool_char(0);
    iw >> bit;
    if (!iw) {
      std::cerr << "invalid data" << std::endl;
      exit(0);
    }
    if (bit.data > 0) {
      decode_dfs(root->right, iw, output);
    } else {
      decode_dfs(root->left, iw, output);
    }
  }
}
