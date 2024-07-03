TARGET=token_parser.out
EXAMPLESTRING=Hello 123 456 vkCppCourse

CXX=g++
CXXFLAGS :=-std=c++20 -Wall -g -Wextra -Wsign-conversion
GTESTFLAGS=-lgtest_main -lgtest -lpthread

SOURCES=TokenParser.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

VALGRIND_OPTS=--leak-check=full --show-leak-kinds=all --track-origins=yes

all: test

build: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: build
	echo -e '$(EXAMPLESTRING)' | ./$(TARGET)

test: TokenParserTest.o TokenParser.o
	$(CXX) $(CXXFLAGS) -o Test.out TokenParserTest.o TokenParser.o $(GTESTFLAGS)
	./Test.out
TokenParserTest.o: TokenParserTest.cpp TokenParser.hpp
	$(CXX) $(CXXFLAGS) -c TokenParserTest.cpp
TokenParser.o: TokenParser.cpp TokenParser.hpp
	$(CXX) $(CXXFLAGS) -c TokenParser.cpp

memory: build
	valgrind $(VALGRIND_OPTS) ./$(TARGET)
st_memory:
	cppcheck $(SOURCES)
clean:
	rm -f *.o $(TARGET) Test.out
