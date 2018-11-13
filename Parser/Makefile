.PHONY: clean distclean default

CC=gcc
CFLAGS=-Wall

default: dana

lexer.c: lexer.l
	flex -s -o lexer.c lexer.l

lexer.o: lexer.c tryparser.h

tryparser.h tryparser.c: tryparser.y
	bison -dv -o tryparser.c tryparser.y

tryparser.o: tryparser.c

dana: lexer.o tryparser.o
	$(CC) $(CFLAGS) -o dana $^ -lfl

clean:
	$(RM) lexer.c tryparser.c tryparser.h tryparser.output *.o *~

distclean: clean
	$(RM) dana
