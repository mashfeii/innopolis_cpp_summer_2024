all: build

build: setup.py cjson.cpp
	chmod +x setup.py && ./setup.py install

run: build
	chmod +x main.py && ./main.py
