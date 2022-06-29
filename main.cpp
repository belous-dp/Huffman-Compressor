//
// Created by Danila Belous on 28.06.2022 at 14:46.
//

#include "huffman/decoder.h"
#include "huffman/encoder.h"
#include <fstream>
#include <iostream>
#include <sstream>
int main() {
  encoder enc = encoder();
//  std::string inp = "acbdcdd";
//  std::string inp = "";
  std::string inp = "aaaaa";
  std::stringstream inps1(inp);
  enc.process_input(inps1);
  enc.build_tree();
  enc.print_codes(std::cout);
  std::ofstream outps;
  outps.open("output.txt");
  enc.print_metadata(outps);
  std::stringstream inps2(inp);
  enc.encode(inps2, outps);
  outps.close();
  std::ifstream inpf1("output.txt");
  decoder dec = decoder();
  dec.scan_metadata(inpf1);
  outps.open("output2.txt");
  dec.print_tree(outps);
  outps.close();
  dec.decode(inpf1, std::cout);
  inpf1.close();
}
