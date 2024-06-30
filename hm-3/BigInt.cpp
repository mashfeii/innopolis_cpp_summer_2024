#include "BigInt.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

BigInt::BigInt(const BigInt& rhs) : sz_(rhs.sz_), num_(new uint16_t[rhs.sz_]) {
  std::cout << "___ Copy constructor ___" << std::endl;

  for (size_t i = 0; i != sz_; ++i) {
    num_[i] = rhs.num_[i];
  }
}
BigInt& BigInt::operator=(const BigInt& rhs) {
  std::cout << "___ Copy-assignment operator ___" << std::endl;

  if (this == &rhs) {
    return *this;
  }

  // Case: different size -> change current size and reallocate
  if (sz_ != rhs.sz_) {
    sz_ = rhs.sz_;
    std::cout << "Alloc" << std::endl;
    num_ = new uint16_t[sz_];
  }
  // Rewrite all the digits
  for (size_t i = 0; i != sz_; ++i) {
    num_[i] = rhs.num_[i];
  }
  return *this;
}
BigInt::BigInt(BigInt&& rhs)
    : sz_(std::move(rhs.sz_)), num_(std::move(rhs.num_)) {
  std::cout << "___ Move operator ___" << std::endl;
  // Avoid double-free
  rhs.num_ = nullptr;
  rhs.sz_ = 0;
}
BigInt& BigInt::operator=(BigInt&& rhs) {
  if (this == &rhs) {
    return *this;
  }

  // Move values into current instance
  delete[] num_;
  sz_ = rhs.sz_;
  num_ = rhs.num_;
  // Clean values of argument instance
  rhs.num_ = nullptr;
  rhs.sz_ = 0;

  return *this;
}
BigInt::~BigInt() {
  delete[] num_;
}

BigInt::BigInt(const std::string& str)
    : sz_(str.size()), num_(new uint16_t[str.size()]) {
  for (size_t i = 0; i != sz_; ++i) {
    num_[i] = str[i] - '0';
  }
}
BigInt::BigInt(uint32_t num) : BigInt(std::to_string(num)) {}

BigInt BigInt::operator-(const BigInt&) const {}
BigInt BigInt::operator-(uint32_t) const {}

BigInt BigInt::operator+(const BigInt& rhs) const {}
BigInt BigInt::operator+(uint32_t) const {}

BigInt BigInt::operator*(const BigInt&) const {}
BigInt BigInt::operator*(uint32_t) const {}

size_t BigInt::size() {
  return sz_;
}

std::ostream& operator<<(std::ostream& os, const BigInt& obj) {
  for (size_t i = 0; i != obj.sz_; ++i) {
    os << obj.num_[i];
  }
  return os;
}

bool operator<(const BigInt& lhs, const BigInt& rhs) {
  if (lhs.sz_ != rhs.sz_)
    return lhs.sz_ < rhs.sz_;

  for (size_t i = 0; i != lhs.sz_; ++i) {
    if (lhs.num_[i] != rhs.num_[i]) {
      return lhs.num_[i] < rhs.num_[i];
    }
  }
  return false;
}
bool operator>(const BigInt& lhs, const BigInt& rhs) {
  return rhs < lhs;
}
bool operator==(const BigInt& lhs, const BigInt& rhs) {
  return !(lhs < rhs) && !(lhs > rhs);
}
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
  return lhs < rhs || lhs > rhs;
}
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
  return rhs > lhs || rhs == lhs;
}
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
  return lhs < rhs || lhs == rhs;
}
