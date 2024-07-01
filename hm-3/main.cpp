#include <iostream>
#include "BigInt.hpp"

int main() {
  BigInt a{0};
  BigInt b{123};
  BigInt c{ std::move(a)};
  b = std::move(c);
}
