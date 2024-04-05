.PHONY: all build-image clean sanitize test

CPPVERSION = 20
CXXFLAGS = -Wall -Wextra -Wshadow -Wconversion -Werror -Wpedantic -std=c++$(CPPVERSION) -O3
CXX = g++

all: test

build-image:
	docker build --pull . -t botas

clean:
	rm -f main san*

main: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main

san-addr-undef: main.cpp
	$(CXX) -std=c++$(CPPVERSION) main.cpp -fsanitize=address -fsanitize=undefined -o san-addr-undef

san-thread: main.cpp
	$(CXX) -std=c++$(CPPVERSION) main.cpp -fsanitize=thread -o san-thread

sanitize: san-addr-undef san-thread
	./san-addr-undef
	./san-thread

test: build-image
	docker run --name scipy-botas -v $(shell pwd):/botas --rm botas 500 100000
