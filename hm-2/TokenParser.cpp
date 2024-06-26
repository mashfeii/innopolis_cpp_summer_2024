#include "TokenParser.hpp"
#include <cstdint>
#include <string>

void TokenParser::setStartCallback(parsingCallback callback) {
  startCallback = callback;
}
void TokenParser::setEndCallback(parsingCallback callback) {
  endCallback = callback;
}
void TokenParser::setDigitTokenCallback(digitParsingCallback callback) {
  digitCallback = callback;
}
void TokenParser::setStringTokenCallback(stringParsingCallback callback) {
  stringCallback = callback;
}
void TokenParser::parse(const std::string& str) {
  size_t token_start = 0;
  size_t token_end = 0;
  size_t token_digits = 0;
  auto is_sep = [](char symb) {
    return symb == ' ' || symb == '\n' || symb == '\t' || symb == '\0';
  };

  for (size_t i = 1; i != str.size() + 1; ++i) {
    if (is_sep(str[i - 1]) && !is_sep(str[i])) {
      token_start = i;
    }
    if (std::isdigit(str[i - 1])) {
      ++token_digits;
    }
    if (is_sep(str[i]) && !is_sep(str[i - 1])) {
      token_end = i;

      std::string current_token =
          str.substr(token_start, token_end - token_start);

      if (token_end - token_start == token_digits && digitCallback) {
        uint64_t digit_token;

        try {
          digit_token = std::stoull(current_token);
          digitCallback(digit_token);
        } catch (...) {
          stringCallback(current_token);
        }
      } else if (stringCallback) {
        stringCallback(current_token);
      }

      token_digits = 0;
    }
  }

  if (startCallback) {
    startCallback();
  }

  if (endCallback) {
    endCallback();
  }
}
