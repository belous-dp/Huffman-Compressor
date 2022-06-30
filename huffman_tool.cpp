//
// Created by Danila Belous on 28.06.2022 at 14:46.
//

#include "huffman/decoder.h"
#include "huffman/encoder.h"
#include <args.hxx>
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
  args::ArgumentParser parser(
      "Tool for compressing and decompressing files with Huffman coding.",
      "Author: Danila Belous.");
  args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
  args::Flag compress(parser, "compress", "compress mode", {'c', "compress"});
  args::Flag decompress(parser, "decompress", "decompress mode",
                        {'d', "decompress"});
  args::ValueFlag<std::string> input(parser, "path", "", {"input"});
  args::ValueFlag<std::string> output(parser, "path", "", {"output"});
  try {
    parser.ParseCLI(argc, argv);
    std::ifstream inp = std::ifstream(args::get(input));
    std::ofstream outp = std::ofstream(args::get(output));
    if (!inp || !outp) {
      throw std::invalid_argument("IOException: cannot open input/output files");
    }
    if (compress) {
      encoder enc = encoder();
      enc.process_input(inp);
      enc.build_tree();
      enc.print_metadata(outp);
      inp.close();
      inp = std::ifstream(args::get(input));
      if (!inp) {
        throw std::invalid_argument("IOException");
      }
      enc.encode(inp, outp);
    } else {
      decoder dec = decoder();
      dec.scan_metadata(inp);
      dec.decode(inp, outp);
    }
    inp.close();
    outp.close();
  } catch (args::Help& e) {
    std::cout << parser;
  } catch (args::Error& e) {
    std::cerr << e.what() << std::endl << parser;
    return 1;
  } catch (std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
  //  std::string inp = "acbdcdd";
  //  std::string inp = "acbdcddddd";
  //  std::string inp = "";
  //  std::string inp = "aaaaa";
  //  std::stringstream inps1(inp);
  //  enc.process_input(inps1);
  //  enc.build_tree();
  //  enc.print_codes(std::cout);
  //  std::ofstream outps;
  //  outps.open("output.txt");
  //  enc.print_metadata(outps);
  //  std::stringstream inps2(inp);
  //  enc.encode(inps2, outps);
  //  outps.close();
  //  std::ifstream inpf1("output.txt");
  //  decoder dec = decoder();
  //  dec.scan_metadata(inpf1);
  //  outps.open("init_data.txt");
  //  dec.print_tree(outps);
  //  outps.close();
  //  dec.decode(inpf1, std::cout);
  //  inpf1.close();
}
