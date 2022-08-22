//
// Created by Danila Belous on 29.06.2022 at 00:26.
//

#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include "encoder.h"
#include "input_wrapper.h"

struct decoder {
  using word = encoder::word;

  decoder();
  ~decoder();

  void process_metadata(std::istream& input);

  void decode(std::istream& input, std::ostream& output) const;

  void print_tree(encoder::node* root, output_wrapper& out) const; // todo debug remove

//private:
  encoder::node* tree;
  uint8_t unused;
  void build_tree_dfs(encoder::node*& root, input_wrapper& iw);
  void decode_dfs(encoder::node* root, input_wrapper& iw, std::ostream& ow) const;
};

#endif // HUFFMAN_DECODER_H
