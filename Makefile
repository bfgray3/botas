.PHONY: all build-image clean sanitize test

CXXFLAGS = -Wall -Wextra -Wshadow -Wconversion -Werror -Wpedantic -std=c++20 -O3
CXX = g++

all: test

build-image:
	docker build --pull . -t botas

main:
	$(CXX) $(CXXFLAGS) main.cpp -o main

clean:
	rm -f main san*

test: build-image
	docker run -v $(shell pwd):/botas --rm botas bash -c "echo scipy && /bin/time python script.py && make main && echo botas && /bin/time ./main"

sanitize:
	$(CXX) $(CXXFLAGS) main.cpp -fsanitize=address -fsanitize=undefined -o san1 && ./san1
	$(CXX) $(CXXFLAGS) main.cpp -fsanitize=thread -o san2 && ./san2
