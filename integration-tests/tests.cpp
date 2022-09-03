#include "../huffman/decoder.h"
#include "../huffman/encoder.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <gmock/gmock.h>
#include <random>
#include <string>

struct bs : bit_sequence {
  std::vector<uint8_t> const& get_bits() const {
    return bits;
  };
};

TEST(bit_sequence, default_ctor) {
  bs s;
  EXPECT_THAT(s.get_bits(), ::testing::ContainerEq(std::vector<uint8_t>()));
  EXPECT_EQ(s.size(), 0);
}

TEST(bit_sequence, copy_ctor) {
  bs s1;
  s1.append_word(234);
  s1.append_bit(1);
  bs s2 = s1;
  EXPECT_THAT(s1.get_bits(), ::testing::ContainerEq(s2.get_bits()));
  EXPECT_EQ(s1.size(), s2.size());
  s1.append_bit(1);
  EXPECT_THAT(s1.get_bits(), ::testing::Not(::testing::ContainerEq(s2.get_bits())));
  EXPECT_NE(s1.size(), s2.size());
}

TEST(bit_sequence, assignment) {
  bs s1;
  s1.append_word(234);
  s1.append_bit(1);
  bs s2;
  s2.append_word(123);
  s2.append_bit(0);
  s2.append_bit(1);
  EXPECT_THAT(s1.get_bits(), ::testing::Not(::testing::ContainerEq(s2.get_bits())));
  EXPECT_NE(s1.size(), s2.size());
  s2 = s1;
  EXPECT_THAT(s1.get_bits(), ::testing::ContainerEq(s2.get_bits()));
  EXPECT_EQ(s1.size(), s2.size());
}

TEST(bit_sequence, swap) {
  bs s1;
  s1.append_word(234);
  s1.append_bit(1);
  bs s2;
  s2.append_word(123);
  s2.append_bit(0);
  s2.append_bit(1);
  std::vector<uint8_t> s1b(s1.get_bits());
  size_t s1s = s1.size();
  std::vector<uint8_t> s2b(s2.get_bits());
  size_t s2s = s2.size();
  s1.swap(s2);
  EXPECT_THAT(s1.get_bits(), ::testing::ContainerEq(s2b));
  EXPECT_EQ(s1.size(), s2s);
  EXPECT_THAT(s2.get_bits(), ::testing::ContainerEq(s1b));
  EXPECT_EQ(s2.size(), s1s);
}

TEST(bit_sequence, words) {
    // 0011011010100101011
    // 00110110 = 108
    //         10100101 = 165
    //                 011 = 6
    bs s;
    s.append_word(108);
    s.append_word(165);
    s.append_word(6);
    EXPECT_EQ(s.word_at(0), 108);
    EXPECT_EQ(s.word_at(bs::WORD_WIDTH), 165);
    EXPECT_EQ(s.word_at(bs::WORD_WIDTH * 2), 6);
}

TEST(bit_sequence, bits) {
  // 0011011010100101011
  // 00110110 = 108
  //         10100101 = 165
  //                 011 = 6
  std::string str = "0011011010100101011";
  bs s;
  for (size_t i = 0; i < str.size(); ++i) {
    uint8_t bit = str[i] - '0';
    s.append_bit(bit);
  }
  EXPECT_EQ(s.size(), str.size());
  for (size_t i = 0; i < str.size(); ++i) {
    uint8_t bit = str[i] - '0';
    EXPECT_EQ(s.bit_at(i), bit);
  }
}

TEST(bit_sequence, pop_back) {
  // 0011011010100101011
  // 00110110 = 108
  //         10100101 = 165
  //                 011 = 6
  std::string str = "0011011010100101011";
  bs s;
  for (size_t i = 0; i < str.size(); ++i) {
    uint8_t bit = str[i] - '0';
    s.append_bit(bit);
  }
  do {
    str.pop_back();
    s.pop_back_bit();
    EXPECT_EQ(s.size(), str.size());
    for (size_t i = 0; i < str.size(); ++i) {
      uint8_t bit = str[i] - '0';
      EXPECT_EQ(s.bit_at(i), bit);
    }
  } while (!str.empty());
  EXPECT_TRUE(s.get_bits().empty());
}

std::string encode_decode(std::string const& s) {
  std::istringstream init_data_input(s);
  encoder enc = encoder();
  enc.process_input(init_data_input);

//  enc.print_codes(std::cout);

  init_data_input = std::istringstream(s);
  std::ostringstream compressed_data_output;
  enc.print_metadata(compressed_data_output);
  enc.encode(init_data_input, compressed_data_output);

//  std::cout << "compressed data output: '" << compressed_data_output.str() << "'" << std::endl;

  decoder dec = decoder();
  std::istringstream compressed_data_input(compressed_data_output.str());
  dec.process_metadata(compressed_data_input);
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

TEST(correctness, englishRussian) {
  std::string s = "aaaaaaaaaaaaaaaaaa"
                  "dddddddddddddddddd"
                  "eeeeeeeeeeeeeeeeee"
                  "fffffffffff"
                  "ggggggggggg"
                  "hhhhhhh"
                  "ффффффф"
                  "ххххххх"
                  "ццццццц"
                  "шшшшшшш"
                  "ччч"
                  "щщщ"
                  "ъъъ"
                  "ыыы"
                  "www"
                  "z"
                  "1"
                  "2"
                  "3"
                  "4"
                  "5"
                  "7";
  EXPECT_EQ(s, encode_decode(s));
}

TEST(correctness, englishRussian4) {
  std::string s = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                  "ddddddddddddddddddddddddddd"
                  "eeeeeeeeeeeeeeeeeeeeeeeeee"
                  "fffffffffffffff"
                  "ggggggggggggggg"
                  "hhhhhhhh"
                  "фффффффф"
                  "ххххххх"
                  "ццццццц"
                  "ччччч"
                  "шшшшшшшш"
                  "щщщщщщщ"
                  "ъъъъъъ"
                  "ыыыыы"
                  "wwww"
                  "z"
                  "1"
                  "2"
                  "3"
                  "4"
                  "5"
                  "7";
  EXPECT_EQ(s, encode_decode(s));
}

TEST(correctness, random) {
  std::mt19937 gen(42);
  std::uniform_int_distribution<> len_distr(0, 10000);
  std::uniform_int_distribution<> word_distr(0, bit_sequence::WORD_MAX_VAL);
  for (size_t i = 0; i < 100; ++i) {
    std::string s;
    size_t len = len_distr(gen);
    for (size_t j = 0; j < len; ++j) {
      s += static_cast<char>(word_distr(gen));
    }
    EXPECT_EQ(s, encode_decode(s));
  }
}
