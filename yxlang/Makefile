# plain simple Makefile to build exprtest

CXX = g++
AR = ar
LEX = flex
YACC = bison

CXXFLAGS = -W -Wall -Wextra -ansi -g -Wno-error=register -std=c++20 -fcoroutines
LDFLAGS = -Wno-error=register -lpthread

HEADERS = driver.h parser.h scanner.h expression.h handler.h \
    y.tab.h FlexLexer.h location.hh position.hh stack.hh \
	coro/awaiters.h  coro/io_context.h  coro/socketawaiters.h  coro/socket.h coro/task.h cohttp.h

all: libyxlang.a

# Generate scanner and parser

parser.cc: parser.yy
	$(YACC) -o parser.cc --defines=parser.h parser.yy

scanner.cc: scanner.ll
	$(LEX) -o scanner.cc scanner.ll

# Implicit rule to compile c++ files

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Link executable

libyxlang.a: parser.o scanner.o driver.o expression.o handler.o coro/socket.o coro/io_context.o cohttp.o
	$(AR) -rcs libyxlang.a parser.o scanner.o driver.o expression.o handler.o coro/socket.o coro/io_context.o cohttp.o

clean:
	rm -f libyxlang.a *.o *~ coro/*.o coro/*~

extraclean: clean
	rm -f parser.cc parser.h scanner.cc
