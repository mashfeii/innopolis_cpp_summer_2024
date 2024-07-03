TARGET=format_output.out

CXX=g++
CXXFLAGS :=-std=c++20 -Wall -g -Wextra -Wsign-conversion
GTESTFLAGS=-lgtest_main -lgtest -lpthread

SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

VALGRIND_OPTS=--leak-check=full --show-leak-kinds=all --track-origins=yes

all: test

build: main.o 
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

main.o: main.cpp FormatOutput.hpp
	$(CXX) $(CXXFLAGS) -c  main.cpp -o main.o 

run: build
	./$(TARGET)

debug: build
	gdb ./$(TARGET)

test: FormatOutputTest.o FormatOutput.hpp
	$(CXX) $(CXXFLAGS) -o Test.out FormatOutputTest.o $(GTESTFLAGS)
	./Test.out
FormatOutputTest.o: FormatOutputTest.cpp FormatOutput.hpp
	$(CXX) $(CXXFLAGS) -c FormatOutputTest.cpp

memory: build
	valgrind $(VALGRIND_OPTS) ./$(TARGET)
st_memory:
	cppcheck $(SOURCES)
clean:
	rm -f *.o $(TARGET) Test.out
