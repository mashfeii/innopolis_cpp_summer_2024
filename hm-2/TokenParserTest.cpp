#include <gtest/gtest.h>
#include <cstdint>
#include "TokenParser.hpp"

class TokenParserTest : public ::testing::Test {
 protected:
  TokenParser* parser;

  size_t tokensCount = 0;
  size_t stringsCount = 0;
  size_t digitsCount = 0;
  std::vector<std::string> strings;
  std::vector<uint64_t> digits;

  std::function<void(const std::string&)> stringTokenCallback =
      [&](const std::string& str) {
        stringsCount++;
        tokensCount++;
        strings.push_back(str);
      };

  std::function<void(uint64_t)> digitTokenCallback = [&](uint64_t digit) {
    digitsCount++;
    tokensCount++;
    digits.push_back(digit);
  };

  std::function<void()> startCallback = nullptr;
  std::function<void()> endCallback = nullptr;

  void SetUp() {
    parser = new TokenParser;
    parser->setStartCallback(startCallback);
    parser->setEndCallback(endCallback);
    parser->setDigitTokenCallback(digitTokenCallback);
    parser->setStringTokenCallback(stringTokenCallback);
  }
  void TearDown() { delete parser; }
};

TEST_F(TokenParserTest, onlySeparators) {
  std::string input_string = "\n \t \t \t \t \t \n\n \t";
  parser->parse(input_string);

  ASSERT_EQ(tokensCount, 0);
  ASSERT_EQ(stringsCount, 0);
  ASSERT_EQ(digitsCount, 0);
}

TEST_F(TokenParserTest, emptyString) {
  std::string input_string = "";
  parser->parse(input_string);

  ASSERT_EQ(tokensCount, 0);
  ASSERT_EQ(stringsCount, 0);
  ASSERT_EQ(digitsCount, 0);
}

TEST_F(TokenParserTest, singleCharString) {
  std::string input_string = "s";
  parser->parse(input_string);

  ASSERT_EQ(tokensCount, 1);
  ASSERT_EQ(stringsCount, 1);
  ASSERT_EQ(digitsCount, 0);
  ASSERT_EQ(*(strings.begin()), "s");
}

TEST_F(TokenParserTest, singleDigitString) {
  std::string input_string = "0";
  parser->parse(input_string);

  ASSERT_EQ(tokensCount, 1);
  ASSERT_EQ(stringsCount, 0);
  ASSERT_EQ(digitsCount, 1);
  ASSERT_EQ(*(digits.begin()), 0);
}

TEST_F(TokenParserTest, onlyCharsString) {
  std::string input_string = "o\tb\n \t \t \t as qw \n sdf\t";
  parser->parse(input_string);

  ASSERT_EQ(tokensCount, 5);
  ASSERT_EQ(stringsCount, 5);
  ASSERT_EQ(digitsCount, 0);

  std::vector<std::string> real_tokens = {"o", "b", "as", "qw", "sdf"};
  for (size_t i = 0; i != real_tokens.size(); ++i) {
    ASSERT_EQ(real_tokens[i], strings[i]);
  }
}

TEST_F(TokenParserTest, onlyDigitsString) {
  std::string input_string = "123\t32\n \n \t\t 23\n 9 3 2";
  parser->parse(input_string);

  ASSERT_EQ(tokensCount, 6);
  ASSERT_EQ(stringsCount, 0);
  ASSERT_EQ(digitsCount, 6);

  std::vector<uint64_t> real_tokens = {123, 32, 23, 9, 3, 2};
  for (size_t i = 0; i != real_tokens.size(); ++i) {
    ASSERT_EQ(real_tokens[i], digits[i]);
  }
}

TEST_F(TokenParserTest, bothDigitsAndStrings) {
  std::string input_string =
      "hello\t \n 123\t \t\t how\n 92  are \t 8379\n\n you";
  parser->parse(input_string);

  ASSERT_EQ(tokensCount, 7);
  ASSERT_EQ(stringsCount, 4);
  ASSERT_EQ(digitsCount, 3);

  std::vector<uint64_t> real_digits_tokens = {123, 92, 8379};
  for (size_t i = 0; i != real_digits_tokens.size(); ++i) {
    ASSERT_EQ(real_digits_tokens[i], digits[i]);
  }

  std::vector<std::string> real_string_tokens = {"hello", "how", "are", "you"};
  for (size_t i = 0; i != real_string_tokens.size(); ++i) {
    ASSERT_EQ(real_string_tokens[i], strings[i]);
  }
}

TEST_F(TokenParserTest, noCallback) {
  std::string input_string = "string but without any callback functions";

  parser->setDigitTokenCallback(nullptr);
  parser->setStringTokenCallback(nullptr);
  parser->parse(input_string);

  ASSERT_EQ(tokensCount, 0);
  ASSERT_EQ(stringsCount, 0);
  ASSERT_EQ(digitsCount, 0);
}

TEST_F(TokenParserTest, digitsOverflow) {
  std::string input_string =
      "9234089274298047230974 930948723094872347092384 723420947237049823490 "
      "80938472309847234978";

  parser->parse(input_string);

  ASSERT_EQ(tokensCount, 4);
  ASSERT_EQ(stringsCount, 4);
  ASSERT_EQ(digitsCount, 0);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
