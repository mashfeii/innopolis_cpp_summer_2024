#include <gtest/gtest.h>
#include "Allocator.h"

TEST(AllocatorTest, PositiveSizeAllocation) {
  Allocator alloc;
  alloc.makeAllocator(1000);
  char* partInMemory = alloc.alloc(250);
  ASSERT_NE(partInMemory, nullptr);
}

TEST(AllocatorTest, ZeroSizeAllocation) {
  Allocator alloc;
  alloc.makeAllocator(0);
  char* partInMemory = alloc.alloc(250);
  ASSERT_EQ(partInMemory, nullptr);
}

TEST(AllocatorTest, SizeOverflow) {
  Allocator alloc;
  alloc.makeAllocator(1200);
  char* partInMemory = alloc.alloc(1201);
  ASSERT_EQ(partInMemory, nullptr);
}

TEST(AllocatorTest, MakeReallocation) {
  Allocator alloc;
  alloc.makeAllocator(1);
  char* firstAllocation = alloc.alloc(1);
  ASSERT_EQ(alloc.size(), 0);
  ASSERT_NE(firstAllocation, nullptr);
  alloc.makeAllocator(250);
  char* secondAllocation = alloc.alloc(250);
  ASSERT_EQ(alloc.size(), 0);
  ASSERT_NE(secondAllocation, nullptr);
}

TEST(AllocatorTest, MultipleFullSizeAllocations) {
  Allocator alloc;
  alloc.makeAllocator(1200);
  char* firstHalf = alloc.alloc(600);
  ASSERT_EQ(alloc.size(), 600);
  char* secondHalf = alloc.alloc(600);
  ASSERT_EQ(alloc.size(), 0);
  char* overflow_part = alloc.alloc(1);
  ASSERT_NE(firstHalf, nullptr);
  ASSERT_NE(secondHalf, nullptr);
  ASSERT_EQ(overflow_part, nullptr);
}

TEST(AllocatorTest, NonInitializedAllocator) {
  Allocator alloc;
  char* emptyMemory = alloc.alloc(250);
  ASSERT_EQ(alloc.size(), 0);
  ASSERT_EQ(emptyMemory, nullptr);
}

TEST(AllocatorTest, ResetAllocator) {
  Allocator alloc;
  alloc.makeAllocator(250);
  char* memoryBeforeReset = alloc.alloc(250);
  ASSERT_NE(memoryBeforeReset, nullptr);

  alloc.reset();
  ASSERT_EQ(alloc.size(), 250);

  char* firstHalfAfterReset = alloc.alloc(125);
  char* secondHalfAfterReset = alloc.alloc(125);
  ASSERT_EQ(alloc.size(), 0);
  ASSERT_NE(firstHalfAfterReset, nullptr);
  ASSERT_NE(secondHalfAfterReset, nullptr);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
