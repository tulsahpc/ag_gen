#!/usr/bin/make -f

CC = clang
CFLAGS = -g -Wall -Wpedantic --std=c99 -DDEBUG
LIBS = -lpq -lhiredis
TARGETS = ag_gen db_test redis_test string_test

AG_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(TARGETS)),$(wildcard ag_*.c)))
DB_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(TARGETS)),$(wildcard db_*.c)))
REDIS_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(TARGETS)),$(wildcard redis_*.c)))
OTHER_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(TARGETS)),$(wildcard util*.c)))

.PHONY: all
all: $(TARGETS)

$(TARGETS):%:%.o $(AG_HELPERS) $(DB_HELPERS) $(REDIS_HELPERS) $(OTHER_HELPERS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -rf *.o *_test ag_gen *.dSYM

.PHONY: get-deps
get-deps:
	sudo apt-get install postgresql postgresql-contrib libpq5 libpq-dev redis-server libhiredis0.13 libhiredis-dev

.PHONY: test
test: db_test redis_test

.PHONY: check
check:
	@cppcheck --std=c99 *.{c,h}
