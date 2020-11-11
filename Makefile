CXX=g++
CXXFLAGS=-std=c++17 -Wall -I. -fPIC
LDFLAGS=

.PHONY: test clean all

all: build/c1 build/lexer_test build/token_test

build/token.o: frontend/token.cpp frontend/token.h
	$(CXX) $(CXXFLAGS) -c frontend/token.cpp -o $@

build/lexer.o: frontend/token.h frontend/lexer.h frontend/lexer.cpp
	$(CXX) $(CXXFLAGS) -c frontend/lexer.cpp -o $@

build/lexer_test.o: frontend/token.h frontend/lexer.h frontend/lexer_test.cpp
	$(CXX) $(CXXFLAGS) -c frontend/lexer_test.cpp -o $@

build/token_test.o: frontend/token.h frontend/token_test.cpp
	$(CXX) $(CXXFLAGS) -c frontend/token_test.cpp -o $@

build/main.o: frontend/token.h frontend/lexer.h main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

build/c1: build/main.o build/lexer.o build/token.o
	$(CXX) $(LDFLAGS) $^ -o $@

build/lexer_test: build/lexer.o build/token.o build/lexer_test.o
	$(CXX) $(LDFLAGS) $^ -o $@

build/token_test: build/token.o build/token_test.o
	$(CXX) $(LDFLAGS) $^ -o $@

test: build/token_test build/lexer_test
	./build/token_test
	./build/lexer_test

clean:
	rm build/*