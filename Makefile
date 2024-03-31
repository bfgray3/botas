.PHONY: all clean sanitize test

CXXFLAGS = -Wall -Wextra -Wshadow -Wconversion -Werror -Wpedantic -std=c++20 -O3
CXX = g++

all: test

clean:
	rm -f a.out san*

test:
	docker build . -t botas
	echo botas && $(CXX) $(CXXFLAGS) main.cpp && /bin/time ./a.out  # FIXME: is this returning an integer instead of double??
	echo scipy && docker run -v $(shell pwd):/botas --rm -it botas /bin/time python script.py

sanitize:
	$(CXX) $(CXXFLAGS) main.cpp -fsanitize=address -fsanitize=undefined -o san1 && ./san1
	$(CXX) $(CXXFLAGS) main.cpp -fsanitize=thread -o san2 && ./san2
