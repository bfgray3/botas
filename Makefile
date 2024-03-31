.PHONY: all build clean sanitize test

CXXFLAGS = -Wall -Wextra -Wshadow -Wconversion -Werror -Wpedantic -std=c++20 -O3
CXX = g++

all: test

build:
	docker build . -t botas

clean:
	rm -f a.out san*

test:
	docker run -v $(shell pwd):/botas --rm -it botas echo scipy && /bin/time python script.py && echo botas && $(CXX) $(CXXFLAGS) main.cpp && /bin/time ./a.out

sanitize:
	$(CXX) $(CXXFLAGS) main.cpp -fsanitize=address -fsanitize=undefined -o san1 && ./san1
	$(CXX) $(CXXFLAGS) main.cpp -fsanitize=thread -o san2 && ./san2
