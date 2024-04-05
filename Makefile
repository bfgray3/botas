.PHONY: all build-image clean sanitize test

CPPVERSION = 20
CXXFLAGS = -Wall -Wextra -Wshadow -Wconversion -Werror -Wpedantic -std=c++$(CPPVERSION) -O3
CXX = g++
MAIN_CPP = main.cpp

all: test

build-image:
	docker build --pull . -t botas

clean:
	rm -f main san*

main: main.cpp
	$(CXX) $(CXXFLAGS) $(MAIN_CPP) -o $@

san-addr-undef: main.cpp
	$(CXX) -std=c++$(CPPVERSION) $(MAIN_CPP) -fsanitize=address -fsanitize=undefined -o $@

san-thread: main.cpp
	$(CXX) -std=c++$(CPPVERSION) $(MAIN_CPP) -fsanitize=thread -o $@

sanitize: san-addr-undef san-thread
	./san-addr-undef
	./san-thread

test: build-image
	docker run --name scipy-botas -v $(shell pwd):/botas --rm botas 500 100000
