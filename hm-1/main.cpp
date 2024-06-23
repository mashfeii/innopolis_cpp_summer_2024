#include <iostream>
#include <string>
#include "Allocator.h"

int main() {
  Allocator alloc;
  alloc.makeAllocator(120);
  char* ptr = alloc.alloc(32);

  std::string* my_string = new (ptr) std::string("Hello, world");
  std::cout << *my_string;
}
