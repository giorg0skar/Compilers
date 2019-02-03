.PHONY: clean distclean default

# export SHELL := /bin/bash

# ifeq ($(shell uname -s),Darwin)
# export PATH := /usr/local/opt/llvm/bin:$(PATH)
# endif

CXXFLAGS=-Wall -g -std=c++11 `llvm-config --cxxflags`
# LDFLAGS=`llvm-config --ldflags --system-libs --libs all`
LDFLAGS=`llvm-config --cxxflags --ldflags --libs all --system-libs`

default: dana

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

parser.o: parser.cpp parser.hpp MYast.hpp

lexer.o: lexer.cpp parser.hpp

MYast.o: MYast.cpp MYast.hpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

dana: lexer.o parser.o MYast.o
	$(CXX) $(CXXFLAGS) -o dana $^ $(LDFLAGS) -lfl

clean:
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output a.* *.o *~

distclean: clean
	$(RM) dana
