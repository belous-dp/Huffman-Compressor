//
// Created by Danila Belous on 29.06.2022 at 00:26.
//

#include "decoder.h"
#include "input_wrapper.h"
#include <cassert>
#include <iostream>

decoder::decoder() : tree(nullptr), nlast_bits(0) {}

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
      throw std::invalid_argument("metadata corrupted");
    }
    nlast_bits = nlb.data >> static_cast<uint8_t>(cool_char::WORD_WIDTH - nlb.nbits);
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
        throw std::invalid_argument("invalid tree");
      }
      root = new encoder::node(w.data);
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

void decoder::decode(std::istream& input, std::ostream& output) {
  auto iw = input_wrapper(input);
  if (iw && iw.eof() && nlast_bits == cool_char::WORD_WIDTH) {
    nlast_bits = 0;
  }
  while (iw && nlast_bits > 0) {
    decode_dfs(tree, iw, output);
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
//      throw std::invalid_argument("invalid data");
      output.flush();
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
