#include "BigInt.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

BigInt::BigInt(const BigInt& rhs)
    : sz_(rhs.sz_), num_(new uint16_t[rhs.sz_]), sign_(rhs.sign_) {
  for (size_t i = 0; i != sz_; ++i) {
    num_[i] = rhs.num_[i];
  }
}
BigInt& BigInt::operator=(const BigInt& rhs) {
  if (this == &rhs) {
    return *this;
  }

  // Case: different size -> change current size and reallocate
  if (sz_ != rhs.sz_) {
    sz_ = rhs.sz_;
    num_ = new uint16_t[sz_];
  }
  // Rewrite all the digits
  for (size_t i = 0; i != sz_; ++i) {
    num_[i] = rhs.num_[i];
  }
  return *this;
}
BigInt::BigInt(BigInt&& rhs)
    : sz_(std::move(rhs.sz_)),
      num_(std::move(rhs.num_)),
      sign_(std::move(rhs.sign_)) {
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
  for (size_t i = sz_ + 1; i-- > 1;) {
    num_[sz_ - i] = str[i - 1] - '0';
  }
}
BigInt::BigInt(uint32_t num) : BigInt(std::to_string(num)) {}

BigInt BigInt::operator-(const BigInt& rhs) const {
  size_t max_len = sz_ > rhs.sz_ ? sz_ : rhs.sz_;
  size_t min_len = sz_ == max_len ? rhs.sz_ : sz_;

  for (size_t i = 0; i != min_len; ++i) {

  }
}
BigInt BigInt::operator-(uint32_t) const {}

BigInt BigInt::operator+(const BigInt& rhs) const {
  size_t max_len = sz_ > rhs.sz_ ? sz_ : rhs.sz_;
  size_t min_len = sz_ == max_len ? rhs.sz_ : sz_;

  BigInt result(std::string(max_len + 1, '0'));
  uint16_t overflow = 0;

  // going through shorter number
  for (size_t i = 0; i != min_len; ++i) {
    result[i] = (num_[i] + rhs[i] + overflow) % 10;
    overflow = (num_[i] + rhs[i] + overflow) / 10;
  }
  // going through longer number if sizes are different
  for (size_t i = min_len; i != max_len; ++i) {
    uint16_t overflow_temp = sz_ > rhs.sz_ ? num_[i] : rhs[i];
    result[i] = (overflow_temp + overflow) % 10;
    overflow = (overflow_temp + overflow) / 10;
  }

  result[max_len] = overflow;
  return result;
}
BigInt BigInt::operator+(uint32_t rhs) const {
  BigInt temp(rhs);
  return this->operator+(temp);
}

BigInt BigInt::operator*(const BigInt&) const {}
BigInt BigInt::operator*(uint32_t) const {}

size_t BigInt::size() {
  return sz_;
}
uint16_t& BigInt::operator[](size_t index) {
  if (index >= sz_) {
    throw std::out_of_range("Incorrect index");
  }
  return num_[index];
}
uint16_t BigInt::operator[](size_t index) const {
  if (index >= sz_) {
    throw std::out_of_range("Incorrect index");
  }
  return num_[index];
}

std::ostream& operator<<(std::ostream& os, const BigInt& obj) {
  size_t i = obj.sz_;
  // omit 'null' digit in case of overflow
  while (obj.num_[i-- - 1] == 0 && i > 0) {
  }
  for (++i; i > 0; --i) {
    os << obj.num_[i - 1];
  }
  return os;
}

bool operator<(const BigInt& lhs, const BigInt& rhs) {
  size_t l_sz = lhs.sz_;
  size_t r_sz = rhs.sz_;

  // omit 'null' digit in case of overflow
  while (lhs.num_[l_sz-- - 1] == 0 && l_sz > 0) {
  }
  while (rhs.num_[r_sz-- - 1] == 0 && r_sz > 0) {
  }

  if (l_sz < r_sz)
    return true;
  if (r_sz < l_sz)
    return false;

  for (size_t i = l_sz; i-- > 0;) {
    if (lhs.num_[l_sz - i] < rhs.num_[r_sz - i]) {
      return true;
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
