TARGET=formatter.out

CXX=g++
CXXFLAGS :=-std=c++20 -Wall -g -Wextra -Wsign-conversion
GTESTFLAGS=-lgtest_main -lgtest -lpthread
VALGRIND_OPTS=--leak-check=full --show-leak-kinds=all --track-origins=yes

all: test

build: main.o
	$(CXX) $(CXXFLAGS) FormatOutput.hpp main.cpp -o $(TARGET)

run: build
	./$(TARGET)

debug: build
	gdb ./$(TARGET)

main.o: main.cpp FormatOutput.hpp
	$(CXX) $(CXXFLAGS) -c FormatOutput.hpp main.cpp

test: FormatOutputTest.o
	$(CXX) $(CXXFLAGS) -o Test.out FormatOutputTest.o FormatOutput.o $(GTESTFLAGS)
	./Test.out
FormatOutputTest.o: FormatOutputTest.cpp FormatOutput.hpp
	$(CXX) $(CXXFLAGS) -c FormatOutputTest.cpp

memory: build
	valgrind $(VALGRIND_OPTS) ./$(TARGET)
st_memory:
	cppcheck $(SOURCES)
clean:
	rm -f *.o $(TARGET) Test.out
