TARGET=big_int.out

CXX=g++
CXXFLAGS :=-std=c++20 -Wall -g -Wextra -Wsign-conversion
GTESTFLAGS=-lgtest_main -lgtest -lpthread

SOURCES=BigInt.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

VALGRIND_OPTS=--leak-check=full --show-leak-kinds=all --track-origins=yes

all: test

build: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: build
	./$(TARGET)

debug: build
	gdb ./$(TARGET)

test: BigIntTest.o BigInt.o
	$(CXX) $(CXXFLAGS) -o Test.out BigIntTest.o BigInt.o $(GTESTFLAGS)
	./Test.out
BigIntTest.o: BigIntTest.cpp BigInt.hpp
	$(CXX) $(CXXFLAGS) -c BigIntTest.cpp
BigInt.o: BigInt.cpp BigInt.hpp
	$(CXX) $(CXXFLAGS) -c BigInt.cpp

memory: build
	valgrind $(VALGRIND_OPTS) ./$(TARGET)
st_memory:
	cppcheck $(SOURCES)
clean:
	rm -f *.o $(TARGET) Test.out
