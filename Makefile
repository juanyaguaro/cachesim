# Copyright 2020 Juan Yaguaro
# Makefile

# ----------- COMPILER FLAGS -----------
CXX = g++
CXX_INCLUDE = -Iinclude
CXX_FLAGS = -std=c++17 -Wall -Werror --pedantic -o

# ----------- CACHESIM FLAGS -----------
SRC  = src/cachesim.cc
BIN = bin/cachesim
RM = rm

# ----------- TESTS FLAGS --------------
TEST_SRC = src/test_generator.cc
TEST_BIN = bin/test_generator

# ----------- WINDOWS -----------
ifeq ($(OS), Windows_NT)
	BIN = bin/cachesim.exe
	TEST_BIN = bin/test_generator.exe
	RM = del
endif

# make
all:
	@echo "Creating cachesim..."
	$(CXX) $(SRC) $(CXX_INCLUDE) $(CXX_FLAGS) $(BIN)
	@echo "cachesim succesfully created..."

#make clean
clean:
	@echo "Removing cachesim..."
	$(RM) $(BIN)
	$(RM) $(TEST_BIN)
	@echo "cachesim succesfully removed."

#make run
run:
	./$(BIN)

#make test_generator
test_generator:
	@echo "Creating test_generator..."
	$(CXX) $(TEST_SRC) $(CXX_INCLUDE) $(CXX_FLAGS) $(TEST_BIN)
	@echo "test_generator succesfully created..."
