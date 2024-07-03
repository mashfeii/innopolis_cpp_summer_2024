#include <gtest/gtest.h>
#include <sstream>
#include "BigInt.hpp"

TEST(BigIntTest, StringConstrucotr) {
  std::stringstream output;

  BigInt string_init{"-123"};
  output << string_init;
  ASSERT_EQ(output.str(), "-123");
  ASSERT_EQ(string_init.size(), 3);
  output.str("");
}
TEST(BigIntTest, UnsignedIntegerConstructor) {
  std::stringstream output;
  BigInt integer_init{246};
  output << integer_init;
  ASSERT_EQ(output.str(), "246");
  ASSERT_EQ(output.str().size(), 3);
  output.str("");
}
TEST(BigIntTest, CopyConstructor) {
  std::stringstream output;

  BigInt string_init{"-123"};
  BigInt copy_init(string_init);
  output << copy_init;
  ASSERT_EQ(output.str(), "-123");
  ASSERT_EQ(string_init.size(), 3);
  output.str("");
}
TEST(BigIntTest, AssignCopyOperator) {
  std::stringstream output;
  BigInt string_init{"120"};
  BigInt assign_copy_init;
  assign_copy_init = string_init;
  output << assign_copy_init;
  ASSERT_EQ(output.str(), "120");
  ASSERT_EQ(output.str().size(), 3);
  output.str("");
}
TEST(BigIntTest, MoveConstructor) {
  std::stringstream output;
}
TEST(BigIntTest, AssignMoveOperator) {
  std::stringstream output;
}
TEST(BigIntTest, InstanceEqualsZero) {
  std::stringstream output;
  BigInt a{"0"};
  BigInt b{0};

  output << a;
  ASSERT_EQ(output.str(), "0");
  output.str("");

  output << a.size();
  ASSERT_EQ(output.str(), "1");
  output.str("");

  output << b;
  ASSERT_EQ(output.str(), "0");
  output.str("");

  output << b.size();
  ASSERT_EQ(output.str(), "1");
  output.str("");
}

TEST(BigIntTest, LessComparingOperator) {
  BigInt positive("123");
  BigInt another_positive("5");
  ASSERT_EQ(another_positive < positive, true);
}
TEST(BigIntTest, MoreComparingOperator) {
  BigInt positive("123");
  BigInt negative("-123");
  ASSERT_EQ(positive > negative, true);
}
TEST(BigIntTest, LessOrEqualComparingOperator) {
  BigInt positive("123");
  BigInt another_positive("122");
  ASSERT_EQ(another_positive <= positive, true);
}
TEST(BigIntTest, MoreOrEqualComparingOperator) {
  BigInt positive("123");
  BigInt negative("-123");
  ASSERT_EQ(positive >= negative, true);
}
TEST(BigIntTest, EqualComparingOperator) {
  BigInt positive("123");
  BigInt equal(123);
  ASSERT_EQ(positive == equal, true);
  ASSERT_EQ(positive != equal, false);
}

TEST(BigIntTest, MoveOperations) {
  BigInt integer("1234567890");
  BigInt moved_in_integer(std::move(integer));
  BigInt operator_move("123");
  operator_move = std::move(moved_in_integer);

  ASSERT_EQ(integer.size(), 0);
  ASSERT_EQ(moved_in_integer.size(), 0);
  ASSERT_EQ(operator_move, 1234567890);
}

TEST(BigIntTest, PositiveSumWithoutOverflow) {
  std::stringstream output;
  BigInt x{"123"};
  BigInt y{"777"};

  output << x + y;
  ASSERT_EQ(output.str(), "900");
  ASSERT_EQ((x + y).size(), 3);
  output.str("");
}
TEST(BigIntTest, PositiveSumWithOverflow) {
  std::stringstream output;
  BigInt x{"123"};
  BigInt y{"877"};

  output << x + y;
  ASSERT_EQ(output.str(), "1000");
  ASSERT_EQ((x + y).size(), 4);
  output.str("");
}

TEST(BigIntTest, PositiveSubtractWithoutBorrow) {
  std::stringstream output;
  BigInt x{"500"};
  BigInt y{"200"};

  output << x - y;
  ASSERT_EQ(output.str(), "300");
  ASSERT_EQ((x - y).size(), 3);
}
TEST(BigIntTest, PositiveSubtractWithBorrow) {
  std::stringstream output;
  BigInt x{"250"};
  BigInt y{"200"};

  output << x - y;
  ASSERT_EQ(output.str(), "50");
  ASSERT_EQ((x - y).size(), 2);
}

TEST(BigIntTest, PositiveProduct) {
  std::stringstream output;
  BigInt x{"50"};
  BigInt y{"200"};

  output << x * y;
  ASSERT_EQ(output.str(), "10000");
}
TEST(BigIntTest, NegativeProduct) {
  std::stringstream output;
  BigInt x{"-50"};
  BigInt y{"-200"};

  output << x * y;
  ASSERT_EQ(output.str(), "10000");
}
TEST(BigIntTest, MixedProduct) {
  std::stringstream output;
  BigInt x{"50"};
  BigInt y{"-200"};

  output << x * y;
  ASSERT_EQ(output.str(), "-10000");
}

TEST(BigIntTest, MixedSignOperations) {
  std::stringstream output;
  BigInt small_positive{120};
  BigInt another_positive{"100"};

  output << small_positive + -another_positive;
  ASSERT_EQ(output.str(), "20");
  output.str("");

  output << -small_positive + another_positive;
  ASSERT_EQ(output.str(), "-20");
  output.str("");

  output << small_positive - -another_positive;
  ASSERT_EQ(output.str(), "220");
  output.str("");

  output << -small_positive - another_positive;
  ASSERT_EQ(output.str(), "-220");
  output.str("");
}
TEST(BigIntTest, ReallyLongNumberConstructing) {
  std::stringstream output;

  BigInt big_number("123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789");
  output << big_number.size();
  ASSERT_EQ(output.str(), "648");
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
