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

void encoder::process_input(std::istream& input) {
  while (input) {
    word w = input.get();
    if (input) {
      process_input(w);
    }
  }
}

void encoder::process_input(encoder::word character) {
  //  frequency[static_cast<size_t>(character)]++;
  frequency[character]++;
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

void encoder::build_tree() {
  //  auto compare = [](encoder::node const* const a,
  //                    encoder::node const* const b) { return a->frq > b->frq;
  //                    };
  auto compare = [](encoder::node const* const a,
                    encoder::node const* const b) {
    if (a->frq != b->frq) {
      return a->frq > b->frq;
    }
    return a->chr < b->chr;
  };
  std::priority_queue<node*, std::vector<node*>, decltype(compare)> q(compare);
  for (size_t i = 0; i < cool_char::WORD_MAX_VAL; ++i) {
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
  if (tree && !tree->left) { // заифаем случаи "ааааааа"
    tree->left = new node(tree->chr);
    tree->right = new node(tree->chr);
  }
  cool_sequence cs;
  build_codes(tree, cs);
}

void encoder::build_codes(encoder::node* root, cool_sequence& cur) {
  if (root) {
    if (root->left) {
      assert(root->right);
      cur.add_bit(0);
      build_codes(root->left, cur);
      cur.remove_bit();
      cur.add_bit(1);
      build_codes(root->right, cur);
      cur.remove_bit();
    } else {
      codes[root->chr] = cur;
    }
  }
}

void encoder::print_metadata(std::ostream& output) {
  output_wrapper ow = output_wrapper(output);
  print_tree_dfs(tree, ow);
  uint8_t nlast_bits = 0;
  for (size_t i = 0; i < cool_char::WORD_MAX_VAL; ++i) {
    nlast_bits += frequency[i] * codes[i].get_nbits();
  }
  if (tree) {
    ow << cool_char(nlast_bits, cool_char::WORD_WIDTH_WIDTH);
  }
}

void encoder::print_tree_dfs(encoder::node* root, output_wrapper& out) {
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

void encoder::print_codes(std::ostream& output) {
//  output << "codes = {\n";
//  for (size_t i = 0; i < cool_char::WORD_MAX_VAL; ++i) {
//    output << "  " << i << " [";
//    for (uint8_t j = 0; j < codes[i].nbits; ++j) {
//      output << (static_cast<uint8_t>(codes[i].data >> (cool_char::WORD_WIDTH - j - 1U)) & 1U);
//    }
//    output << "]\n";
//  }
//  output << "}\n";
}

void encoder::encode(std::istream& input, std::ostream& output) {
  output_wrapper ow = output_wrapper(output);
  while (input) {
    word w = input.get();
    if (input) {
      ow << codes[w];
    }
  }
}

std::vector<cool_sequence> encoder::get_codes() {
  return {codes.begin(), codes.end()};
}
