.PHONY: test

# TODO: add -Wconversion
CXXFLAGS = -Wall -Wextra -Wshadow -Werror -Wpedantic -std=c++23 -O3
CXX = g++

test:
	echo botas && $(CXX) $(CXXFLAGS) main.cpp && ./a.out  # FIXME: is this returning an integer instead of double??
	echo scipy && python3 script.py	
