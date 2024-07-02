#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include "TokenParser.hpp"

int main() {
  TokenParser parser;
  std::string line;

  auto stringCallback = [](const std::string& str) {
    std::cout << "------ \e[1mString token:\e[0m ------\n";
    std::cout << std::setw(14 + (str.size() + 16) / 2);
    std::cout << "\e[41m" + str + "\e[0m";
    std::cout << "\n---------------------------\n" << std::endl;
  };
  auto digitCallback = [](uint64_t digit) {
    std::cout << "------ \e[1mDigit token:\e[0m ------\n";
    std::cout << std::setw(16);
    std::cout << "\e[46m" << digit << "\e[0m";
    std::cout << "\n---------------------------\n" << std::endl;
  };

  parser.setStringTokenCallback(stringCallback);
  parser.setDigitTokenCallback(digitCallback);

  while (std::getline(std::cin, line)) {
    parser.parse(line);
  }
}
