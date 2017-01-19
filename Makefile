#!/usr/bin/make -f

SRC_DIR := src
BIN_DIR := bin
VPATH := .:$(SRC_DIR)

CXX := g++
CXXFLAGS := -g -Wall -Wpedantic --std=c++11 -I$(SRC_DIR)

LIBS := -lm -lpq -lhiredis

BIN := $(BIN_DIR)/main

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
HEADERS := $(wildcard $(SRC_DIR)/*.hpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(SRC_DIR)/%.o,$(SRCS))

all: dir $(BIN)

dir:
	@mkdir -p bin

$(BIN): $(BIN_DIR)/% : $(SRC_DIR)/%.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(OBJS): $(SRC_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: test
test: all
	@./tests.sh

clean:
	@rm -rf bin docs $(SRC_DIR)/*.o
