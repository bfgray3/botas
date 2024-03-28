.PHONY: all clean test

CXXFLAGS = -Wall -Wextra -Wshadow -Wconversion -Werror -Wpedantic -std=c++20 -O3
CXX = g++

all: test

clean:
	rm a.out

test:
	echo botas && $(CXX) $(CXXFLAGS) main.cpp && /bin/time ./a.out  # FIXME: is this returning an integer instead of double??
	echo scipy && /bin/time python3 script.py
