#include <iostream>
#include <string>
#include "FormatOutput.hpp"

int main() {
  std::cout << format("Hello") << std::endl;
  std::cout << format("My name is {0}", "Timofei") << std::endl;
  std::cout << format("I am {0} years old, study in {1} university", 19, "Innopolis") << std::endl;
}
