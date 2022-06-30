#include "../huffman/decoder.h"
#include "../huffman/encoder.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <gmock/gmock.h>
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

TEST(cool_char, addZeroBit3) {
  cool_char cc = cool_char(3, 3);
  cool_char res = cc.add_bit(0);
  EXPECT_EQ(res.data, 0b01100000);
  EXPECT_EQ(res.nbits, 4);
}

TEST(cool_char, addOneBit0) {
  cool_char cc;
  cool_char res = cc.add_bit(1);
  EXPECT_EQ(res.data, 1U << (cool_char::WORD_WIDTH - 1U));
  EXPECT_EQ(res.nbits, 1);
}

TEST(cool_char, addOneBit1) {
  cool_char cc = 1;
  cool_char res = cc.add_bit(1);
  EXPECT_EQ(res.data, 1U << (cool_char::WORD_WIDTH - 1U) |
                          1U << (cool_char::WORD_WIDTH - 2U));
  EXPECT_EQ(res.nbits, 2);
}

TEST(cool_char, addOneBit3) {
  cool_char cc = cool_char(3, 3);
  cool_char res = cc.add_bit(1);
  EXPECT_EQ(res.data, 0b01110000);
  EXPECT_EQ(res.nbits, 4);
}

TEST(cool_char, eqOperator) {
  cool_char cc1 = cool_char(3, 3);
  cool_char cc2 = cool_char(3, 3);
  cool_char cc3 = cool_char(2, 3);
  cool_char cc4 = cool_char(3, 2);
  EXPECT_EQ(cc1, cc2);
  EXPECT_NE(cc1, cc3);
  EXPECT_NE(cc1, cc4);
  EXPECT_NE(cc3, cc4);
}

TEST(encoder, processOneChar) {
  std::vector<cool_sequence> correct(cool_char::WORD_MAX_VAL);
  for (cool_char::word i = 0; i < cool_char::WORD_MAX_VAL; ++i) {
    encoder enc = encoder();
    enc.process_input(i);
    enc.build_tree();
    std::vector<cool_sequence> codes = enc.get_codes();
    correct[i].add_bit(1);
    EXPECT_THAT(codes, ::testing::ContainerEq(correct));
    correct[i].remove_bit();
  }
}

std::string encode_decode(std::string const& s) {
  std::istringstream init_data_input(s);
  encoder enc = encoder();
  enc.process_input(init_data_input);
  enc.build_tree();
//  enc.print_codes(std::cout);
  init_data_input = std::istringstream(s);
  std::ostringstream compressed_data_output;
  enc.print_metadata(compressed_data_output);
  enc.encode(init_data_input, compressed_data_output);
  decoder dec = decoder();
  std::istringstream compressed_data_input(compressed_data_output.str());
  dec.scan_metadata(compressed_data_input);
  std::ostringstream decompressed_data_output;
  dec.decode(compressed_data_input, decompressed_data_output);
  return decompressed_data_output.str();
}

TEST(correctness, empty) {
  std::string s;
  EXPECT_EQ(s, encode_decode(s));
}

TEST(correctness, small) {
  std::string s = "acbdcdd";
  EXPECT_EQ(s, encode_decode(s));
}

TEST(correctness, fit) {
  std::string s = "acbdcddddd";
  EXPECT_EQ(s, encode_decode(s));
}

TEST(correctness, oneLetter) {
  std::string s = "zzzzzzz";
  EXPECT_EQ(s, encode_decode(s));
}

TEST(correctness, handRandom) {
  std::string s = "askdfhi2h1onaksnd2asldfn3";
  EXPECT_EQ(s, encode_decode(s));
}

TEST(correctness, longRandom) {
  std::string s = "askdfhi2hsjasldfkjalsdjkflasjdf;alksjdfl;akjsdf1029jd0912j3s012js0a;dsf-02398u&y9!*Y(!*#1onaksnd2asldfn3";
  EXPECT_EQ(s, encode_decode(s));
}

//TEST(correctness, random) {
//  std::mt19937 gen(42);
//  std::uniform_int_distribution<> chr(0, cool_char::WORD_MAX_VAL);
//  for (size_t i = 0; i < 100; ++i) {
//    auto val = static_cast<cool_char::word>(rnd(gen));
//    cool_char cc = cool_char(val, cool_char::WORD_WIDTH);
//    EXPECT_EQ(cc.data, val);
//    EXPECT_EQ(cc.nbits, cool_char::WORD_WIDTH);
//  }
//  std::string s = "zzzzzzz";
//  EXPECT_EQ(s, encode_decode(s));
//}
