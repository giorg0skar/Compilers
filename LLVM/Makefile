.PHONY: clean distclean default

# export SHELL := /bin/bash

# ifeq ($(shell uname -s),Darwin)
# export PATH := /usr/local/opt/llvm/bin:$(PATH)
# endif

CXXFLAGS=-Wall -g -std=c++11 `llvm-config --cxxflags`
LDFLAGS=`llvm-config --cxxflags --ldflags --libs all --system-libs`

all: dana libs

libs: 
	cd edsger_lib; bash libs.sh

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

parser.o: parser.cpp parser.hpp ast.hpp
	$(CXX) $(CXXFLAGS) -c parser.cpp

lexer.o: lexer.cpp parser.hpp
	$(CXX) $(CXXFLAGS) -c lexer.cpp

ast.o: ast.cpp ast.hpp
	$(CXX) $(CXXFLAGS) -c ast.cpp

symbol.o: symbol.c symbol.h
	gcc -c -Wall -g symbol.c

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<



dana: lexer.o parser.o ast.o symbol.o error.o general.o
	$(CXX) $(CXXFLAGS) -o dana $^ $(LDFLAGS)

clean:
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output a.* *.o *~

distclean: clean
	$(RM) dana
