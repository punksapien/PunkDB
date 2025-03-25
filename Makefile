# simple makefile for learned index db

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -Iinclude
TARGET = bin/server

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

SRCS = $(SRC_DIR)/server.cpp $(SRC_DIR)/db.cpp $(SRC_DIR)/learned_index.cpp
OBJS = $(BUILD_DIR)/server.o $(BUILD_DIR)/db.o $(BUILD_DIR)/learned_index.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p bin

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run
