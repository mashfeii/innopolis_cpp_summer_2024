#include <iostream>
#include "BigInt.hpp"

int main() {
  BigInt x = 100;
  BigInt y("1");

  std::cout << x - y;
}
