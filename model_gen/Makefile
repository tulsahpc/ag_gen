#!/bin/usr/make -f

CC = clang++
CFLAGS = -g -Wall -Wpedantic --std=c++14

LIBS =

SRC = main.cpp netMod.cpp atkPat.cpp
OBJS = $(SRC:.c=.o)

default: all
all: model_gen
model_gen: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	rm -rf *.o model_gen.* model_gen
