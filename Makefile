#!/usr/bin/make -f

CC = clang
CFLAGS = -g -Wall -Wpedantic --std=c99

LIBS = -lpq

SRC = db_test.c ag_asset.c ag_facts.c ag_network.c \
		db_util.c
OBJS = $(SRC:.c=.o)

default: all

all: db_test redis_test

db_test: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

REDIS_TEST_SRC = redis_test.c ag_redisconnect.c ag_asset.c \
		ag_redisclient.c db_util.c
REDIS_TEST_OBJS = $(REDIS_TEST_SRC:.c=.o)
redis_test: $(REDIS_TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(REDIS_TEST_OBJS) -lpq -lhiredis

.PHONY: clean
clean:
	rm -f *.o db_test redis_test

.PHONY: get-deps
get-deps:
	sudo apt-get install postgresql postgresql-contrib libpq5 libpq-dev redis-server libhiredis0.13 libhiredis-dev

.PHONY: test
test:
