#pragma once
#include <cstdint>
#include <functional>
#include <string>

class TokenParser {
 private:
  using parsingCallback = std::function<void()>;
  using stringParsingCallback = std::function<void(const std::string&)>;
  using digitParsingCallback = std::function<void(uint64_t)>;

  parsingCallback startCallback = nullptr;
  parsingCallback endCallback = nullptr;
  digitParsingCallback digitCallback = nullptr;
  stringParsingCallback stringCallback = nullptr;

 public:
  TokenParser() = default;
  // Setting up callback-function before parsing
  void setStartCallback(parsingCallback callback);
  // Setting up callback-function after parsing
  void setEndCallback(parsingCallback callback);
  // Setting up callback-function for digit parsing
  void setDigitTokenCallback(digitParsingCallback callback);
  // Setting up callback-function for string parsing
  void setStringTokenCallback(stringParsingCallback callback);

  void parse(const std::string& string_to_parse);
};
