# Name of the target executable
TARGET := myapp.out

# Source files (apply pattern to find all the cpp files)
# SOURCES := $(wildcard ((?!Test).)*.cpp)
SOURCES := Allocator.cpp main.cpp

# Object files (change extension name from cpp to o)
OBJECTS := $(SOURCES:.cpp=.o)

# Input file
# INPUT := input.txt

# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -Wall -g -Wextra -Wsign-conversion

# Gtest flags
GTESTFLAGS := -lgtest_main -lgtest -lpthread

# Linker flags
LDFLAGS :=

# Valgrind options
VALGRIND_OPTS := --leak-check=full --show-leak-kinds=all --track-origins=yes

# Default make target
all: test

# Build step
build: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^
# g++ -o myapp.out Allocator.o main.o

# Compile step for source files (first several steps - make object files from sources)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
# g++ -Wall -g -Wextra -Wsign-conversion -c Allocator.cpp -o Allocator.o
# g++ -Wall -g -Wextra -Wsign-conversion -c main.cpp -o main.o

# Run step
run: build
	./$(TARGET)

# Memory check step
memory: build
	valgrind $(VALGRIND_OPTS) ./$(TARGET)

st_memory:
	cppcheck $(SOURCES)

# Using Google Test
test: AllocatorTest.o Allocator.o
	$(CXX) $(CXXFLAGS) -o Test.out AllocatorTest.o Allocator.o $(GTESTFLAGS)
	./Test.out

AllocatorTest.o: AllocatorTest.cpp Allocator.h
	$(CXX) $(CXXFLAGS) -c AllocatorTest.cpp

Allocator.o: Allocator.cpp Allocator.h
	$(CXX) $(CXXFLAGS) -c Allocator.cpp

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET) AllocatorTest.o Test.out

.PHONY: all build run memory clean
