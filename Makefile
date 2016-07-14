#!/usr/bin/make -f

SRC_DIR := src
BIN_DIR := bin
VPATH := .:$(SRC_DIR)

CC := clang
CFLAGS := -Wall -Wpedantic --std=c99 -I$(SRC_DIR)
LIBS := -lm -lpq -lhiredis

TARGETS := ag_gen
TARGETS := $(BIN_DIR)/$(TARGETS)
TESTS := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(wildcard $(SRC_DIR)/*_test.c))

# Union of executables
EXECS := $(sort $(TARGETS) $(TESTS))

AG_HELPERS := $(filter-out $(patsubst $(BIN_DIR)/%,$(SRC_DIR)/%.o,$(EXECS)),$(patsubst %.c,%.o,$(wildcard $(SRC_DIR)/ag_*.c)))
DB_HELPERS := $(filter-out $(patsubst $(BIN_DIR)/%,$(SRC_DIR)/%.o,$(EXECS)),$(patsubst %.c,%.o,$(wildcard $(SRC_DIR)/db_*.c)))
REDIS_HELPERS := $(filter-out $(patsubst $(BIN_DIR)/%,$(SRC_DIR)/%.o,$(EXECS)),$(patsubst %.c,%.o,$(wildcard $(SRC_DIR)/redis_*.c)))
OTHER_HELPERS := $(filter-out $(patsubst $(BIN_DIR)/%,$(SRC_DIR)/%.o,$(EXECS)),$(patsubst %.c,%.o,$(wildcard $(SRC_DIR)/util*.c)))

# $(info $(TARGETS))
# $(info $(TESTS))
# $(info $(EXECS))

# $(info $(AG_HELPERS))
# $(info $(DB_HELPERS))
# $(info $(REDIS_HELPERS))
# $(info $(OTHER_HELPERS))

.PHONY: default
default: dir debug

.PHONY: build
build: dir $(TARGETS)

.PHONY: dir
dir:
	@mkdir -p bin

.PHONY: debug
debug: CFLAGS += -DDEBUG -g
debug: $(EXECS)

$(EXECS):$(BIN_DIR)/%: $(SRC_DIR)/%.o $(AG_HELPERS) $(DB_HELPERS) $(REDIS_HELPERS) $(OTHER_HELPERS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -rf bin docs $(SRC_DIR)/*.o

.PHONY: test
test: dir $(TESTS)
	@./tests.sh

.PHONY: check
check:
	@cppcheck --std=c99 *.c
	@cppcheck --std=c99 *.h

.PHONY: docs
docs:
	@doxygen
