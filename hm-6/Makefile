all: test

build: setup.py cjson.cpp string_utils.hpp
	chmod +x setup.py && ./setup.py install

test: build
	pytest --durations=0 --no-header -v -rA ./main.py

speed: build
	chmod +x speed_test.py && ./speed_test.py
