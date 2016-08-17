#!/usr/bin/make -f

SRC_DIR := src
BIN_DIR := bin
VPATH := .:$(SRC_DIR)

CC := clang
CFLAGS := -Wall -Wpedantic --std=c99 -I$(SRC_DIR)
LIBS := -lm -lpq -lhiredis

TARGETS := ag_gen
TARGETS := $(BIN_DIR)/$(TARGETS)
TESTS := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(wildcard $(SRC_DIR)/test_*.c))

# Union of executables
EXECS := $(sort $(TARGETS) $(TESTS))
SRCS := $(filter-out $(patsubst $(BIN_DIR)/%,$(SRC_DIR)/%.c,$(EXECS)),$(wildcard $(SRC_DIR)/*.c))
OBJS := $(patsubst $(SRC_DIR)/%.c,$(SRC_DIR)/%.o,$(SRCS))

# $(info $(TARGETS))
# $(info $(TESTS))
# $(info $(EXECS))
# $(info $(SRCS))
# $(info $(OBJS))

.PHONY: default
default: dir debug

.PHONY: build
build: dir $(TARGETS)

.PHONY: dir
dir:
	@mkdir -p bin

.PHONY: debug
debug: CFLAGS += -DDEBUG -g
debug: $(TARGETS)

.PHONY: debugall
debugall: CFLAGS += -DDEBUG -g
debugall: $(EXECS)

$(EXECS):$(BIN_DIR)/%: $(SRC_DIR)/%.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -rf bin docs $(SRC_DIR)/*.o

.PHONY: test
test: dir debug
	@./tests.sh

.PHONY: fulltest
fulltest: dir debugall $(EXECS)
	@./tests.sh all

.PHONY: docs
docs:
	@doxygen
