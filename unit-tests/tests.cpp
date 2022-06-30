#include "../huffman/encoder.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <random>

TEST(cool_char, defaultCtor) {
  cool_char cc;
  EXPECT_EQ(cc.data, 0);
  EXPECT_EQ(cc.nbits, 0);
}

TEST(cool_char, zeroBitCtor) {
  cool_char cc = 0;
  EXPECT_EQ(cc.data, 0);
  EXPECT_EQ(cc.nbits, 1);
}

TEST(cool_char, oneBitCtor) {
  cool_char cc = 1;
  EXPECT_EQ(cc.data, 1U << (cool_char::WORD_WIDTH - 1U));
  EXPECT_EQ(cc.nbits, 1);
}

TEST(cool_char, threeBitsCtor) {
  cool_char cc = cool_char(3, 3);
  EXPECT_EQ(cc.data, 0b01100000);
  EXPECT_EQ(cc.nbits, 3);
}

TEST(cool_char, wordCtor) {
  std::mt19937 gen(42);
  std::uniform_int_distribution<> rnd(0, cool_char::WORD_MAX_VAL);
  for (size_t i = 0; i < 100; ++i) {
    auto val = static_cast<cool_char::word>(rnd(gen));
    cool_char cc = cool_char(val, cool_char::WORD_WIDTH);
    EXPECT_EQ(cc.data, val);
    EXPECT_EQ(cc.nbits, cool_char::WORD_WIDTH);
  }
}

TEST(cool_char, addZeroBit0) {
  cool_char cc;
  cool_char res = cc.add_bit(0);
  EXPECT_EQ(res.data, 0);
  EXPECT_EQ(res.nbits, 1);
}

TEST(cool_char, addZeroBit1) {
  cool_char cc = 1;
  cool_char res = cc.add_bit(0);
  EXPECT_EQ(res.data, 1U << (cool_char::WORD_WIDTH - 1U));
  EXPECT_EQ(res.nbits, 2);
}

//TEST(cool_char, addZeroBit3) {
//  cool_char cc = 1;
//  cc.add_bit(0);
//  EXPECT_EQ(cc.data, 1U << (cool_char::WORD_WIDTH - 1U));
//  EXPECT_EQ(cc.nbits, 2);
//}

TEST(encoder, processesOneChar) {
  encoder enc = encoder();
  enc.process_input('a');
  enc.build_tree();
  std::vector<cool_char> codes = enc.get_codes();
  std::vector<cool_char> correct(cool_char::WORD_MAX_VAL);
  //  EXPECT_EQ()
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
