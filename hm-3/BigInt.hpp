#pragma once

#include <cstdint>
#include <ostream>
#include <string>

class BigInt {
 private:
  size_t sz_ = 0;
  uint16_t* num_ = nullptr;
  bool sign_ = false;

 public:
  // Rule of 5
  BigInt() = default;
  BigInt(const BigInt&);
  BigInt& operator=(const BigInt&);
  BigInt(BigInt&&);
  BigInt& operator=(BigInt&&);
  ~BigInt();

  // Custom constructors
  BigInt(const std::string&);
  BigInt(uint32_t);

  // Unary arithmetic operations
  // methods are const since we return brand new instance
  BigInt operator-(const BigInt&) const;
  BigInt operator-(uint32_t) const;

  BigInt operator+(const BigInt&) const;
  BigInt operator+(uint32_t) const;

  BigInt operator*(const BigInt&) const;
  BigInt operator*(uint32_t) const;

  uint16_t& operator[](size_t);
  uint16_t operator[](size_t) const;

  // Comparison operations
  friend bool operator<(const BigInt& lhs, const BigInt& rhs);
  friend bool operator>(const BigInt& lhs, const BigInt& rhs);
  friend bool operator==(const BigInt& lhs, const BigInt& rhs);
  friend bool operator!=(const BigInt& lhs, const BigInt& rhs);
  friend bool operator<=(const BigInt& lhs, const BigInt& rhs);
  friend bool operator>=(const BigInt& lhs, const BigInt& rhs);

  size_t size();
  // Output
  friend std::ostream& operator<<(std::ostream&, const BigInt&);
};
