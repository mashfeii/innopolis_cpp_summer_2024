#include "Allocator.h"

Allocator::Allocator() : ptr_(nullptr), shift_(0), sz_(0) {}

void Allocator::makeAllocator(size_t max_size) {
  if (max_size == 0)
    return;
  if (ptr_)
    delete[] ptr_;

  try {
    ptr_ = new char[max_size];
  } catch (...) {
    throw;
  }
  // Reset shift for non-first allocation
  shift_ = 0;
  sz_ = max_size;
}

char* Allocator::alloc(size_t size) {
  // Lack of memory || Unitialized memory
  if ((size + shift_ > sz_) || sz_ == 0 || !ptr_) {
    return nullptr;
  }

  shift_ += size;
  return ptr_ + shift_ - size;
}

void Allocator::reset() {
  shift_ = 0;
}

size_t Allocator::size() {
  return sz_ - shift_;
}

Allocator::~Allocator() {
  delete[] ptr_;
}
