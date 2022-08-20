//
// Created by Danila Belous on 28.06.2022 at 12:43.
//

#include "encoder.h"
#include "output_wrapper.h"
#include <cassert>
#include <iostream>
#include <queue>

encoder::encoder() : codes({}), frequency({}), tree(nullptr) {
  assert(std::is_unsigned_v<word>);
}

encoder::~encoder() {
  destroy_tree(tree);
}

void destroy_tree(encoder::node* root) {
  if (root) {
    destroy_tree(root->left);
    destroy_tree(root->right);
    root->~node();
    delete root;
  }
}

bool is_leaf(encoder::node* pNode) {
  if (pNode->left != nullptr) {
    assert(pNode->right != nullptr);
    return false;
  }
  assert(pNode->right == nullptr);
  return true;
}

encoder::node::node(encoder::node* left, encoder::node* right)
    : left(left), right(right), chr{}, frq(0) {
  if (left) {
    frq += left->frq;
  }
  if (right) {
    frq += right->frq;
  }
}

encoder::node::node(encoder::word chr, size_t frq)
    : left(nullptr), right(nullptr), chr(chr), frq(frq) {}

encoder::node::node(encoder::word chr) : node(chr, 0) {}



void encoder::process_input(std::istream& input) {
  collect_statistics(input);
  build_tree();
}

void encoder::collect_statistics(std::istream& input) {
  while (input) {
    word w = input.get();
    if (input) {
      collect_statistics(w);
    }
  }
}

void encoder::collect_statistics(encoder::word character) {
  frequency[character]++;
}

void encoder::build_tree() {
  auto compare = [](encoder::node const* const a,
                    encoder::node const* const b) {
    if (a->frq != b->frq) {
      return a->frq > b->frq;
    }
    return a->chr < b->chr;
  };
  std::priority_queue<node*, std::vector<node*>, decltype(compare)> q(compare); // todo maybe smart pointers?
  for (size_t i = 0; i < bit_sequence::WORD_MAX_VAL; ++i) {
    if (frequency[i] > 0) {
      q.push(new node(i, frequency[i]));
    }
  }
  while (q.size() > 1) {
    node* a = q.top();
    q.pop();
    node* b = q.top();
    q.pop();
    q.push(new node(a, b));
  }
  if (!q.empty()) {
    tree = q.top();
    q.pop();
  }
  bit_sequence cs;
  if (tree && is_leaf(tree)) { // "aaa", "bbb", etc.
    cs.append_bit(0);
  }
  build_codes(tree, cs);
}

void encoder::build_codes(encoder::node* root, bit_sequence& cur) {
  if (root) {
    if (!is_leaf(root)) {
      cur.append_bit(0);
      build_codes(root->left, cur);
      cur.pop_back_bit();
      cur.append_bit(1);
      build_codes(root->right, cur);
      cur.pop_back_bit();
    } else {
      codes[root->chr] = cur;
    }
  }
}

void encoder::print_metadata(std::ostream& output) const {
  auto ow = output_wrapper(output);
  print_tree(tree, ow);
  uint8_t len = 0;
  for (size_t i = 0; i < bit_sequence::WORD_MAX_VAL; ++i) {
    len += frequency[i] * codes[i].size();
  }
  if (tree) {
    len %= bit_sequence::WORD_WIDTH;
    uint8_t unused = (len == 0 ? 0 : bit_sequence::WORD_WIDTH - len);
    ow.print_bit(unused & 1).print_bit((unused >> 1) & 1).print_bit((unused >> 2) & 1);
  }
}

void encoder::print_tree(encoder::node* root, output_wrapper& out) const {
  // идея: на внутренних вершинах выводим 0, на листьях -- 1 и код символа
  if (root) {
    if (!is_leaf(root)) {
      out.print_bit(0);
      print_tree(root->left, out);
      print_tree(root->right, out);
    } else {
      out.print_bit(1).print_word(root->chr);
    }
  }
}

void encoder::encode(std::istream& input, std::ostream& output) const {
  output_wrapper ow = output_wrapper(output);
  while (input) {
    word w = input.get();
    if (input) {
      ow.print_bit_sequence(codes[w]);
    }
  }
}

std::vector<bit_sequence> encoder::get_codes() {
  return {codes.begin(), codes.end()};
}

void encoder::print_codes(std::ostream& output) {
  output << "codes = {\n";
  for (size_t i = 0; i < bit_sequence::WORD_MAX_VAL; ++i) {
    output << "  " << i << " [";
    for (size_t j = 0; j < codes[i].size(); ++j) {
      output << static_cast<uint16_t>(codes[i].bit_at(j));
    }
    output << "]\n";
  }
  output << "}\n";
}
