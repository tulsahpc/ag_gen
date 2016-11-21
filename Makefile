#!/usr/bin/make -f

SRC_DIR := src
BIN_DIR := bin
VPATH := .:$(SRC_DIR)

CC := clang
CXX := clang++

CFLAGS := -g -Wall -Wpedantic --std=c99 -I$(SRC_DIR)
CXXFLAGS := -g -Wall -Wpedantic --std=c++14 -I$(SRC_DIR)

LIBS := -lm -lpq -lhiredis

BIN := $(BIN_DIR)/ag_gen
BIN_STATIC := $(BIN_DIR)/util_db.a $(BIN_DIR)/util_odometer.a

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(SRC_DIR)/%.o,$(SRCS))

C_SRC := $(wildcard $(SRC_DIR)/*.c)
C_OBJS := $(patsubst $(SRC_DIR)/%.c,$(SRC_DIR)/%.o,$(C_SRCS))

all: dir $(BIN_STATIC) $(BIN)

dir:
	@mkdir -p bin

$(BIN): $(BIN_DIR)/% : $(SRC_DIR)/%.o $(OBJS) $(BIN_STATIC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(OBJS): $(SRC_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

$(BIN_STATIC): $(BIN_DIR)/%.a : $(SRC_DIR)/%.o
	ar rcs $@ $^

$(C_OBJS): $(SRC_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: test
test: all
	@./tests.sh

clean:
	@rm -rf bin docs $(SRC_DIR)/*.o
