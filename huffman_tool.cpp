//
// Created by Danila Belous on 28.06.2022 at 14:46.
//

#include "decoder.h"
#include "encoder.h"
#include <args.hxx>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
  args::ArgumentParser parser(
      "Tool for compressing and decompressing files with Huffman coding. Files sizes may exceed the memory size.",
      "Author: Danila Belous.");
  args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
  args::Flag compress(parser, "compress", "Compress mode", {'c', "compress"});
  args::Flag decompress(parser, "decompress", "Decompress mode",
                        {'d', "decompress"});
  args::ValueFlag<std::string> input(parser, "path", "Path to the input file.", {"input"});
  args::ValueFlag<std::string> output(parser, "path", "Path to the output file.", {"output"});
  try {
    parser.ParseCLI(argc, argv);
    std::ifstream inp = std::ifstream(args::get(input));
    if (!inp) {
      throw std::ios_base::failure("cannot open input file");
    }
    std::ofstream outp = std::ofstream(args::get(output));
    outp.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    if (!outp) {
      throw std::ios_base::failure("cannot open output file");
    }
    if (compress) {
      if (decompress) {
        throw std::invalid_argument("wrong arguments: compress and decompress at the same time is prohibited");
      }
      encoder enc = encoder();
      enc.process_input(inp);
      enc.print_metadata(outp);
      inp.clear();
      inp.seekg(0);
      enc.encode(inp, outp);
    } else {
      decoder dec = decoder();
      dec.process_metadata(inp);
      dec.decode(inp, outp);
    }
  } catch (args::Help& e) {
    std::cout << parser;
  } catch (args::Error& e) {
    std::cerr << e.what() << std::endl << parser;
    return 1;
  } catch (std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
