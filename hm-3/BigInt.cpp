#include "BigInt.hpp"
#include <sys/types.h>
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
    sign_ = rhs.sign_;
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
    : sz_(str[0] == '-' ? str.size() - 1 : str.size()),
      num_(new uint16_t[sz_]),
      sign_(str[0] == '-') {
  for (size_t i = sz_ + 1; i-- > 1;) {
    num_[sz_ - i] = (str[0] == '-' ? str[i] : str[i - 1]) - '0';
  }
}
BigInt::BigInt(uint32_t num) : BigInt(std::to_string(num)) {}

BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
  if (lhs.sign_ && !rhs.sign_) {
    BigInt temp = lhs;
    temp.sign_ = false;
    return -(temp + rhs);
  } else if (!lhs.sign_ && rhs.sign_) {
    BigInt temp = rhs;
    temp.sign_ = false;
    return lhs + temp;
  } else if (lhs.sign_ && rhs.sign_) {
    BigInt lhs_temp = lhs;
    BigInt rhs_temp = rhs;
    lhs_temp.sign_ = false;
    rhs_temp.sign_ = false;
    return rhs_temp - lhs_temp;
  }

  const BigInt& minuend = lhs > rhs ? lhs : rhs;
  const BigInt& subtrahend = minuend == lhs ? rhs : lhs;
  size_t max_size = minuend.sz_;
  BigInt result(std::string(max_size, '0'));

  uint8_t borrow = 0;
  for (size_t i = 0; i != max_size; ++i) {
    uint16_t right = (i < subtrahend.sz_) ? subtrahend[i] : 0;

    if (minuend[i] - borrow < right) {
      result[i] = 10 + minuend[i] - borrow - right;
      borrow = 1;
    } else {
      result[i] = minuend[i] - borrow - right;
      borrow = 0;
    }
  }

  while (result.sz_ > 1 && result[result.sz_ - 1] == 0) {
    --result.sz_;
  }

  if (minuend != lhs)
    result.sign_ = true;

  return result;
}
BigInt operator-(const BigInt& lhs, uint32_t rhs) {
  BigInt temp(rhs);
  return lhs - rhs;
}

BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
  if (lhs.sign_ && !rhs.sign_) {
    BigInt temp = lhs;
    temp.sign_ = false;
    return rhs - temp;

  } else if (!lhs.sign_ && rhs.sign_) {
    BigInt temp = rhs;
    temp.sign_ = false;
    return lhs - temp;
  }

  size_t max_len = lhs.sz_ > rhs.sz_ ? lhs.sz_ : rhs.sz_;

  BigInt result(std::string(max_len + 1, '0'));
  uint16_t overflow = 0;

  for (size_t i = 0; i != max_len; ++i) {
    uint16_t left = (i < lhs.sz_) ? lhs[i] : 0;
    uint16_t right = (i < rhs.sz_) ? rhs[i] : 0;
    result[i] = (left + right + overflow) % 10;
    overflow = (left + right + overflow) / 10;
  }

  result[max_len] = overflow;

  // Remove leading zeros
  while (result.sz_ > 1 && result.num_[result.sz_ - 1] == 0) {
    --result.sz_;
  }

  if (lhs.sign_ && rhs.sign_)
    result.sign_ = true;

  return result;
}
BigInt operator+(const BigInt& lhs, uint32_t rhs) {
  BigInt temp(rhs);
  return lhs + temp;
}

BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
  const BigInt& longer = lhs.sz_ < rhs.sz_ ? rhs : lhs;
  const BigInt& shorter = lhs.sz_ < rhs.sz_ ? lhs : rhs;

  size_t max_len = longer.sz_;
  BigInt result(std::string(max_len * max_len + 1, '0'));

  if (lhs.sign_ != rhs.sign_) {
    result.sign_ = true;
  }

  for (size_t i = 0; i != shorter.sz_; ++i) {
    BigInt temp(std::string(max_len * max_len + 1, '0'));
    uint16_t overflow = 0;

    for (size_t j = 0; j != longer.sz_; ++j) {
      uint16_t product = shorter[i] * longer[j] + overflow;
      temp[j + i] = product % 10;
      overflow = product / 10;
    }

    temp[max_len + i] = overflow;

    while (temp.sz_ > 1 && temp[temp.sz_ - 1] == 0) {
      --temp.sz_;
    }
    result = result + temp;
  }

  while (result.sz_ > 1 && result[result.sz_ - 1] == 0) {
    --result.sz_;
  }
  return result;
}
BigInt operator*(const BigInt&, uint32_t) {}

size_t BigInt::size() const {
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
  if (obj.sign_)
    os << "-";
  for (size_t i = obj.sz_; i-- > 0;) {
    os << obj.num_[i];
  }
  return os;
}

BigInt BigInt::operator-() const {
  BigInt result(*this);
  result.sign_ = true;
  return result;
}

bool operator<(const BigInt& lhs, const BigInt& rhs) {
  if (lhs.sign_ != rhs.sign_) {
    return lhs.sign_ > rhs.sign_;
  }

  size_t lsz = lhs.sz_;
  size_t rsz = rhs.sz_;

  if (lhs.sign_) {
    if (lsz != rsz)
      return lsz > rsz;
    for (size_t i = 0; i != lsz; ++i) {
      if (lhs.num_[i] > rhs.num_[i])
        return true;
    }
    return false;
  }
  if (!lhs.sign_) {
    if (lsz != rsz)
      return lsz < rsz;
    for (size_t i = 0; i != lsz; ++i) {
      if (lhs.num_[i] < rhs.num_[i])
        return true;
    }
    return false;
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
  return lhs > rhs || rhs == lhs;
}
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
  return lhs < rhs || lhs == rhs;
}
