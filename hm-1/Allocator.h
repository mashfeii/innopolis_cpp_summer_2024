#pragma once

#include <cstddef>

class Allocator {
 private:
  char* ptr_;
  size_t shift_;
  size_t sz_;

 public:
  Allocator(); 
  void makeAllocator(size_t max_size);
  char* alloc(size_t size);
  void reset();
  size_t size();
  ~Allocator();
};
