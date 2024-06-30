TARGET=main.out
CXX=g++
CXXFLAGS=-std=c++20 -Wall 

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

MEM_OPTS=--leak-check=full --show-leak-kinds=all --track-origins=yes

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

memory: $(TARGET)
	valgrind $(MEM_OPTS) ./$(TARGET)

clean:
	rm -f *.o $(TARGET)
