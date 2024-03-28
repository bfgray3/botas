.PHONY: test

CXXFLAGS = -Wall -Wextra -Wshadow -Wconversion -Werror -Wpedantic -std=c++23 -O3
CXX = g++

test:
	echo botas && $(CXX) $(CXXFLAGS) main.cpp && /bin/time ./a.out  # FIXME: is this returning an integer instead of double??
	echo scipy && /bin/time python3 script.py
