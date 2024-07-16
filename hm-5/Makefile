TARGET=avl.out

CXX=g++
CXXFLAGS :=-std=c++20 -Wall -g -Wextra -Wsign-conversion
GTESTFLAGS=-lgtest_main -lgtest -lpthread

SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

VALGRIND_OPTS=--leak-check=full --show-leak-kinds=all --track-origins=yes

all: test

build: main.o
	$(CXX) -o $(TARGET) main.o

main.o: main.cpp avl.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

run: build
	./$(TARGET)

test: avltest.o
	$(CXX) $(CXXFLAGS) -o test.out avltest.o $(GTESTFLAGS)
	./test.out
avltest.o: avltest.cpp avl.hpp
	$(CXX) $(CXXFLAGS) -c avltest.cpp

memory: build
	valgrind $(VALGRIND_OPTS) ./$(TARGET)
st_memory:
	cppcheck $(SOURCES)
clean:
	rm -f *.o $(TARGET) test.out
