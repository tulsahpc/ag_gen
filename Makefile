#!/usr/bin/make -f

SHELL = /bin/sh
srcdir = .

CC = clang
CFLAGS = -g -Wall -Wpedantic --std=c99

LIBS = -lpq

SRC = db_test.c ag_asset.c ag_facts.c ag_network.c \
		db_util.c
OBJS = $(SRC:.c=.o)

db_test: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o db_test

.PHONY: test
test:
