#!/usr/bin/make -f

SRC_DIR := src
BIN_DIR := bin
VPATH := .:$(SRC_DIR)

CC := clang
CXX := clang++

CFLAGS := -Wall -Wpedantic --std=c99 -I$(SRC_DIR)
CXXFLAGS := -Wall -Wpedantic --std=c++14 -I$(SRC_DIR)

LIBS := -lm -lpq -lhiredis

BIN := ag_gen
BIN := $(patsubst %,$(BIN_DIR)/%,$(BIN))

BIN_S := $(SRC_DIR)/util_db.c $(SRC_DIR)/util_odometer.c
# BIN_S := $(wildcard $(SRC_DIR)/*.c)
BIN_S := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.a,$(BIN_S))

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(SRC_DIR)/%.o,$(SRCS))

C_SRC := $(wildcard $(SRC_DIR)/*.c)
C_OBJS := $(patsubst $(SRC_DIR)/%.c,$(SRC_DIR)/%.o,$(C_SRCS))

default: dir $(BIN_S) $(BIN)

dir:
	@mkdir -p bin

$(BIN): $(BIN_DIR)/% : $(SRC_DIR)/%.o $(OBJS) $(BIN_S)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(OBJS): $(SRC_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

$(BIN_S): $(BIN_DIR)/%.a : $(SRC_DIR)/%.o
	ar rcs $@ $^

$(C_OBJS): $(SRC_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@rm -rf bin docs $(SRC_DIR)/*.o
