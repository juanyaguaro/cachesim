# Copyright 2020 Juan Yaguaro
# Makefile

# ----------- COMPILER FLAGS -----------
CXX = g++
CXX_INCLUDE = -Iinclude
CXX_FLAGS = -std=c++17 -Wall -Werror --pedantic -o

# ----------- ENVIRONMENT FLAGS -----------
SRC  = src/cachesim.cc
BIN = cachesim
RM = rm

# ----------- WINDOWS -----------
ifeq ($(OS), Windows_NT)
	BIN = cachesim.exe
	RM = del
endif

# make
all:
	$(CXX) $(SRC) $(CXX_INCLUDE) $(CXX_FLAGS) $(BIN)

#make clean
clean:
	@echo "Removing cachesim..."
	$(RM) $(BIN)
	@echo "cachesim succesfully removed."

#make run
run:
	./$(BIN)

#make test
test:
	@echo "Tests to be added soon."
