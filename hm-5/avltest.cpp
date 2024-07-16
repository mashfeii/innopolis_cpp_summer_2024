#include <gtest/gtest.h>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include "avl.hpp"

TEST(AVLTesting, defaultCreationBoolOperations) {
  avl<std::string, uint32_t> tree;

  ASSERT_EQ(tree.empty(), true);
  ASSERT_EQ(tree.size(), 0);
  ASSERT_EQ(tree.height(), 0);
}
TEST(AVLTesting, listInitializerCreation) {
  avl<int, int> tree = {{50, 50}, {40, 40}, {30, 30}, {20, 20}, {10, 10}};
  /*    40
       /  \
     20   50
    /  \
  10   30         */

  ASSERT_EQ(tree.empty(), false);
  ASSERT_EQ(tree.size(), 5);
  ASSERT_EQ(tree.height(), 3);
}

TEST(AVLTesting, treeInsertion) {
  avl<int, int> tree = {{50, 50}, {40, 40}, {30, 30}, {20, 20}, {10, 10}};
  tree.insert({60, 60});
  /*    40
       /  \
     20   50
    /  \   \
  10   30  60      */

  ASSERT_EQ(tree.size(), 6);
  ASSERT_EQ(tree.height(), 3);
}
TEST(AVLTesting, largeInsertionBalance) {
  avl<int, int> tree;

  for (int i = 0; i < 200000; ++i) {
    tree.insert({i, i});
  }

  ASSERT_EQ(tree.height(), 18);
  ASSERT_EQ(tree.size(), 200000);
}
TEST(AVLTesting, treeDeletion) {
  avl<int, int> tree = {{50, 50}, {40, 40}, {30, 30}, {20, 20}, {10, 10}};
  tree.erase(40);
  /*    20
       /  \
     10   50
         /
        30       */

  std::ostringstream s;
  for (auto iter = tree.begin(); iter != tree.end(); ++iter) {
    s << *iter;
  }
  std::string output = "10203050";
  ASSERT_EQ(output, s.str());
  ASSERT_EQ(tree.size(), 4);
}

TEST(AVLTesting, acessOperatorCreation) {
  avl<int, int> tree = {{50, 50}, {40, 40}, {30, 30}, {20, 20}, {10, 10}};
  tree[5] = 25;

  /*         20
           /    \
          10    40
         /     /  \
      5(25)  30   50 */

  ASSERT_EQ(tree.size(), 6);
  ASSERT_EQ(tree.height(), 3);
  ASSERT_EQ(tree[5], 25);
}
TEST(AVLTesting, safeAcessOperator) {
  avl<int, int> tree = {{50, 50}, {40, 40}, {30, 30}, {20, 20}, {10, 10}};
  const avl<int, int> ctree = {
      {50, 50}, {40, 40}, {30, 30}, {20, 20}, {10, 10}};

  EXPECT_THROW(
      {
        try {
          tree.at(120);
        } catch (const std::out_of_range& e) {
          EXPECT_STREQ("No element with such key", e.what());
          throw;
        }
      },
      std::out_of_range);

  EXPECT_THROW(
      {
        try {
          tree.at(120);
        } catch (const std::out_of_range& e) {
          EXPECT_STREQ("No element with such key", e.what());
          throw;
        }
      },
      std::out_of_range);
}

TEST(AVLTesting, forwardIteration) {
  avl<int, int> tree = {{50, 50}, {40, 40}, {30, 30}, {20, 20}, {10, 10}};

  std::ostringstream s;
  for (auto iter = tree.begin(); iter != tree.end(); ++iter) {
    s << *iter;
  }
  std::string output = "1020304050";
  ASSERT_EQ(output, s.str());
}
TEST(AVLTesting, reverseIteration) {
  avl<int, int> tree = {{50, 50}, {40, 40}, {30, 30}, {20, 20}, {10, 10}};

  std::ostringstream s;
  for (auto iter = tree.rbegin(); iter != tree.rend(); ++iter) {
    s << *iter;
  }
  std::string output = "5040302010";
  ASSERT_EQ(output, s.str());
}

TEST(AVLTesting, clearContent) {
  avl<int, int> tree = {{50, 50}, {40, 40}, {30, 30}, {20, 20}, {10, 10}};

  tree.clear();

  ASSERT_EQ(tree.height(), 0);
  ASSERT_EQ(tree.size(), 0);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
