//
// Created by Danila Belous on 29.06.2022 at 00:26.
//

#include "decoder.h"
#include "input_wrapper.h"
#include <cassert>
#include <iostream>

decoder::decoder() : codes({}), tree(nullptr), nlast_bits(0) {}

decoder::~decoder() {
  destroy_tree(tree);
}

void decoder::scan_metadata(std::istream& input) {
  auto iw = input_wrapper(input);
  if (iw && !iw.eof()) {
    build_tree_dfs(tree, iw);
    cool_char nlb = cool_char(0, cool_char::WORD_WIDTH_WIDTH);
    iw >> nlb;
    if (!iw) {
      std::cerr << "metadata corrupted" << std::endl;
      exit(0);
    }
    nlast_bits = nlb.data >> (cool_char::WORD_WIDTH - nlb.nbits);
    if (nlast_bits == 0) {
      nlast_bits = cool_char::WORD_WIDTH;
    }
  }
}

void decoder::build_tree_dfs(encoder::node*& root, input_wrapper& iw) {
  auto bit = cool_char(0);
  iw >> bit;
  if (iw) {
    if (bit.data > 0) {
      auto w = cool_char(0, cool_char::WORD_WIDTH);
      iw >> w;
      if (!iw) {
        std::cerr << "invalid tree" << std::endl;
        exit(0);
      }
      root = new encoder::node(w.data);
    } else {
      root = new encoder::node(nullptr, nullptr);
      build_tree_dfs(root->left, iw);
      build_tree_dfs(root->right, iw);
      if (!root->right) {
        std::cerr << "invalid tree" << std::endl;
        exit(0);
      }
    }
  }
}

void decoder::decode(std::istream& input, std::ostream& output) {
  auto iw = input_wrapper(input);
  if (iw && iw.eof() && nlast_bits == cool_char::WORD_WIDTH) {
    nlast_bits = 0;
  }
  while (iw && nlast_bits > 0) {
    decode_dfs(tree, iw, output);
  }
}

void decoder::print_tree(std::ostream& output) { // todo убрать
  output_wrapper ow = output_wrapper(output);
  print_tree_dfs(tree, ow);
//  output.flush();
//  output << nlast_bits;
//  output.flush();
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
    if (!iw || (iw.eof() && nlast_bits == 0)) {
      std::cerr << "invalid data" << std::endl;
      exit(0);
    }
    if (iw.eof()) {
      nlast_bits--;
    }
    if (bit.data > 0) {
      decode_dfs(root->right, iw, output);
    } else {
      decode_dfs(root->left, iw, output);
    }
  }
}
