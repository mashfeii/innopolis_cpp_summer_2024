#include <gtest/gtest.h>
#include "BigInt.hpp"

TEST(BigIntTest, CreatingInstanceAndComparing) {
  BigInt string_init{"123"};
  BigInt integer_init{246};
  BigInt copy_init(string_init);
  BigInt assign_copy_init;
  assign_copy_init = string_init;

  ASSERT_EQ(string_init, 123);
  ASSERT_EQ(integer_init, 246);
  ASSERT_EQ(copy_init, 123);
  ASSERT_EQ(assign_copy_init, 123);
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

TEST(BigIntTest, PlusOperator) {
  BigInt a("2342343423");
  BigInt b("65876903");

  BigInt d{"123"};
  BigInt e{"7"};
  BigInt f{77};

  ASSERT_EQ(a + b, 2408220326);
  ASSERT_EQ(d + e, 130);
  ASSERT_EQ(d + f, 200);
}

TEST(BigIntTest, InstanceEqualsZero) {
  BigInt a{"0"};
  BigInt b{0};

  ASSERT_EQ(a, 0);
  ASSERT_EQ(b, 0);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
