#!/usr/bin/make -f

CC = clang
CFLAGS := -Wall -Wpedantic --std=c99
LIBS = -lpq -lhiredis

TARGETS = ag_gen
TESTS = db_test redis_test string_test

# Union of executables
EXECS = $(sort $(TARGETS) $(TESTS))

AG_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(EXECS)),$(wildcard ag_*.c)))
DB_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(EXECS)),$(wildcard db_*.c)))
REDIS_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(EXECS)),$(wildcard redis_*.c)))
OTHER_HELPERS := $(patsubst %.c,%.o,$(filter-out $(addsuffix .c,$(EXECS)),$(wildcard util*.c)))

.PHONY: all
all: $(TARGETS)

debug: CFLAGS += -DDEBUG -g
debug: $(TARGETS)

$(EXECS):%:%.o $(AG_HELPERS) $(DB_HELPERS) $(REDIS_HELPERS) $(OTHER_HELPERS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -rf *.o *_test ag_gen *.dSYM

.PHONY: test
test: $(TESTS)
	@echo "\n***** Database Tests"
	@./db_test
	@echo "\n***** Redis Tests"
	@./redis_test
	@echo "\n***** String Library Tests"
	@./string_test

.PHONY: check
check:
	@cppcheck --std=c99 *.c
	@cppcheck --std=c99 *.h
