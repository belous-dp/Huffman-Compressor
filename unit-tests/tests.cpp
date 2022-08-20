#include "../huffman/decoder.h"
#include "../huffman/encoder.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <gmock/gmock.h>
#include <random>

struct bs : bit_sequence {
  std::vector<uint8_t>& get_bits() {
    return bits;
  };
  uint8_t& get_nlb() {
    return nlb;
  }
};

TEST(bit_sequence, defaultCtor) {
  bs s;
  EXPECT_THAT(s.get_bits(), ::testing::ContainerEq(std::vector<uint8_t>()));
  EXPECT_EQ(s.get_nlb(), 0);
}

TEST(bit_sequence, copyCtor) {
  bs s1;
  s1.append_word(234);
  s1.append_bit(1);
  bs s2 = s1;
  EXPECT_THAT(s1.get_bits(), ::testing::ContainerEq(s2.get_bits()));
  EXPECT_EQ(s1.get_nlb(), s2.get_nlb());
  s1.append_bit(1);
  EXPECT_THAT(s1.get_bits(), ::testing::Not(::testing::ContainerEq(s2.get_bits())));
  EXPECT_NE(s1.get_nlb(), s2.get_nlb());
}

TEST(bit_sequence, assignment) {
  bs s1;
  s1.append_word(234);
  s1.append_bit(1);
  bs s2;
  s2.append_word(123);
  s2.append_bit(0);
  EXPECT_THAT(s1.get_bits(), ::testing::Not(::testing::ContainerEq(s2.get_bits())));
  EXPECT_NE(s1.get_nlb(), s2.get_nlb());
  s2 = s1;
  EXPECT_THAT(s1.get_bits(), ::testing::ContainerEq(s2.get_bits()));
  EXPECT_EQ(s1.get_nlb(), s2.get_nlb());
  s1.append_bit(1);
  EXPECT_THAT(s1.get_bits(), ::testing::Not(::testing::ContainerEq(s2.get_bits())));
  EXPECT_NE(s1.get_nlb(), s2.get_nlb());
}
//
//TEST(bit_sequence, swap) {
//  bs s1;
//  s1.append_word(234);
//  s1.append_bit(1);
//  bs s2;
//  s2.append_word(123);
//  s2.append_bit(0);
//  std::vector<uint8_t> s1b(s1.get_bits());
//  std::vector<uint8_t> s1b(s1.get_bits());
//  EXPECT_THAT(s1.get_bits(), ::testing::Not(::testing::ContainerEq(s2.get_bits())));
//  EXPECT_NE(s1.get_nlb(), s2.get_nlb());
//  s2 = s1;
//  EXPECT_THAT(s1.get_bits(), ::testing::ContainerEq(s2.get_bits()));
//  EXPECT_EQ(s1.get_nlb(), s2.get_nlb());
//  s1.append_bit(0);
//  EXPECT_THAT(s1.get_bits(), ::testing::Not(::testing::ContainerEq(s2.get_bits())));
//  EXPECT_NE(s1.get_nlb(), s2.get_nlb());
//}

//TEST(bit_sequence, appendBit0) {
//
//}

//TEST(bit_sequence, wordCtor) {
//  std::mt19937 gen(42);
//  std::uniform_int_distribution<> rnd(0, cool_char::WORD_MAX_VAL);
//  for (size_t i = 0; i < 100; ++i) {
//    auto val = static_cast<cool_char::word>(rnd(gen));
//    cool_char cc = cool_char(val, cool_char::WORD_WIDTH);
//    EXPECT_EQ(cc.data, val);
//    EXPECT_EQ(cc.nbits, cool_char::WORD_WIDTH);
//  }
//}

//TEST(encoder, processOneChar) {
//  std::vector<cool_sequence> correct(cool_char::WORD_MAX_VAL);
//  for (cool_char::word i = 0; i < cool_char::WORD_MAX_VAL; ++i) {
//    encoder enc = encoder();
//    enc.collect_statistics(i);
//    enc.build_tree();
//    std::vector<cool_sequence> codes = enc.get_codes();
//    correct[i].add_bit(1);
//    EXPECT_THAT(codes, ::testing::ContainerEq(correct));
//    correct[i].remove_bit();
//  }
//}

std::string encode_decode(std::string const& s) {
  std::istringstream init_data_input(s);
  encoder enc = encoder();
  enc.collect_statistics(init_data_input);
  enc.build_tree();
  enc.print_codes(std::cout);
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
//  return "";
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

TEST(correctness, russian) {
  std::string s = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя1234567890";
  EXPECT_EQ(s, encode_decode(s));
}

TEST(correctness, russianLong) {
  std::string s = "ааааааааааааааааааааааааааааааааа"
                  "бббббббббббббббббббббббббббббббб"
                  "ввввввввввввввввввввввввввввввв"
                  "гггггггггггггггггггггггггггггг"
                  "ддддддддддддддддддддддддддддд"
                  "ееееееееееееееееееееееееееее"
                  "ёёёёёёёёёёёёёёёёёёёёёёёёёёё"
                  "жжжжжжжжжжжжжжжжжжжжжжжжжж"
                  "ззззззззззззззззззззззззз"
                  "ииииииииииииииииииииииии"
                  "ййййййййййййййййййййййй"
                  "кккккккккккккккккккккк"
                  "ллллллллллллллллллллл"
                  "мммммммммммммммммммм"
                  "ннннннннннннннннннн"
                  "оооооооооооооооооо"
                  "ппппппппппппппппп"
                  "рррррррррррррррр"
                  "ссссссссссссссс"
                  "тттттттттттттт"
                  "ууууууууууууу"
                  "фффффффффффф"
                  "ххххххххххх"
                  "цццццццццц"
                  "ччччччччч"
                  "шшшшшшшш"
                  "щщщщщщщ"
                  "ъъъъъъ"
                  "ыыыыы"
                  "ьььь"
                  "эээ"
                  "юю"
                  "я"
                  "1"
                  "2"
                  "3"
                  "4"
                  "5"
                  "6"
                  "7"
                  "8"
                  "9"
                  "0";
  EXPECT_EQ(s, encode_decode(s));
}

TEST(correctness, medium) {
  std::string s = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                  "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
                  "ccccccccccccccccccccccccccccccc"
                  "гггггггггггггггггггггггггггггг"
                  "ддддддддддддддддддддддддддддд"
                  "ееееееееееееееееееееееееееее"
                  "ёёёёёёёёёёёёёёёёёёёёёёёёёёё"
                  "жжжжжжжжжжжжжжжжжжжжжжжжжж"
                  "ззззззззззззззззззззззззз"
                  "ииииииииииииииииииииииии"
                  "ййййййййййййййййййййййй"
                  "ьььь"
                  "эээ"
                  "юю"
                  "я"
                  "1"
                  "2"
                  "3"
                  "4"
                  "5"
                  "6"
                  "7"
                  "8"
                  "9"
                  "0";
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
