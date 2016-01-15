###########################################################
# Makefile composed by Jiang Yu-Kuan 2005/3/14 ~ 2005/5/21
# Platform: ANSI C / GCC enviroment
# Purpose: Unit Tests
#

CC = gcc

MODULES = ToyUnit Bitmap Queue
TARGETS = $(MODULES) doc
BIN = $(addsuffix _test,$(MODULES))

ToyUnit_OBJS = ToyUnit_test.o
Bitmap_OBJS = Bitmap_test.o Bitmap.o
Queue_OBJS = Queue_test.o Queue.o

W0 = -Wall -Wextra -pedantic -Wdeclaration-after-statement -Wundef -Wwrite-strings
W1 = -Wbad-function-cast -Wcast-qual -Wredundant-decls #-Wunreachable-code
CFLAGS = -std=c9x -DDEBUG $(W0) $(W1) -I"./include"


utest: $(MODULES)
	for bin in $(BIN); do \
	    echo; \
	    echo; \
	    echo "./$$bin" ;  \
	    ./$$bin; \
	    echo; \
	done

all: $(TARGETS)

Bitmap_test: Bitmap
	$@

ToyUnit: $(ToyUnit_OBJS)
	$(CC) -o $@_test $(CFLAGS) $(ToyUnit_OBJS)

Bitmap: $(Bitmap_OBJS)
	$(CC) -o $@_test $(CFLAGS) $(Bitmap_OBJS)

Queue: $(Queue_OBJS)
	$(CC) -o $@_test $(CFLAGS) $(Queue_OBJS)

doc:
	doxygen

.SUFFIXES: .c .o
.c.o:
	$(CC) -c $< $(CFLAGS)


.PHONY : cleanobj cleanbin cleandoc clean
cleanobj:
	rm -f *.o
cleanbin:
	rm -f $(BIN)
	rm -f $(addsuffix .exe,$(BIN))
cleandoc:
	rm -f -r html
clean: cleanobj cleanbin cleandoc
