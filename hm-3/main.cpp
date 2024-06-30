#include <iostream>
#include "BigInt.hpp"

int main() {
  BigInt a{"123"};
  BigInt b{"929"};

  BigInt c = a + b;
  std::cout << c << std::endl;
}
