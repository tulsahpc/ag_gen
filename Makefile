#!/usr/bin/make -f

CC = clang
CFLAGS := -Wall -Wpedantic --std=c99
LIBS = -lm -lpq -lhiredis

TARGETS = ag_gen
TESTS = $(basename $(wildcard *_test.c))

# Union of executables
EXECS = $(sort $(TARGETS) $(TESTS))

AG_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(EXECS)),$(wildcard ag_*.c)))
DB_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(EXECS)),$(wildcard db_*.c)))
REDIS_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(EXECS)),$(wildcard redis_*.c)))
OTHER_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(EXECS)),$(wildcard util*.c)))

.PHONY: default
default: build

.PHONY: build
build: $(TARGETS)

debug: CFLAGS += -DDEBUG -g
debug: $(TARGETS) $(TESTS)

$(EXECS):%:%.o $(AG_HELPERS) $(DB_HELPERS) $(REDIS_HELPERS) $(OTHER_HELPERS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -rf *.o *_test ag_gen *.dSYM docs/

.PHONY: test
test: $(TESTS)
	@./tests.sh

.PHONY: check
check:
	@cppcheck --std=c99 *.c
	@cppcheck --std=c99 *.h

.PHONY: docs
docs:
	@doxygen
