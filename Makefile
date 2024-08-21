# plain simple Makefile to build exprtest

CXX = g++
AR = ar
LEX = flex
YACC = bison

CXXFLAGS = -W -Wall -Wextra -ansi -g -Wno-error=register -std=c++20 -fcoroutines
LDFLAGS = -Wno-error=register -lpthread
INCLUDE=-I./ -I./yxhelper/ -I./yxlang/

HEADERS = yxhelper/context.h yxhelper/httphandler.h yxhelper/yxhelper.h

all: exprtest

exprtest: exprtest.o yxhelper/yxhelper.o
	$(CXX) $(LDFLAGS) -o $@ exprtest.o yxhelper/yxhelper.o -L./yxlang -lyxlang

.cpp.o:
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c $^ -o $@ 

.cc.o:
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c $^ -o $@

clean:
	rm -f *~ yxhelper/*.o yxhelper/*~ exprtest.o exprtest

