# Personal Makefile Template.
CXX = g++
CXX_FLAGS = -std=c++17 -Wconversion -Wall -Werror -Wextra -pedantic
OPT = -O3 -DNDEBUG
DEBUG = -g3 -DDEBUG

HEAD = circular_buffer
EXE = test

# Expand the linked file names into lists of .cpp and .o files.
LINKED_CPP = $(foreach file, $(LINKED_FILES), $(file).cpp)
LINKED_O = $(foreach file, $(LINKED_FILES), $(file).o)

# Build optimized version.
release : clean $(HEAD).h
	$(CXX) $(CXX_FLAGS) $(OPT) -c $(EXE).cpp
	$(CXX) $(CXX_FLAGS) $(OPT) $(EXE).o -o $(EXE)

# Build debugging version.
debug : clean $(HEAD).h
	$(CXX) $(CXX_FLAGS) $(OPT) -c $(EXE).cpp
	$(CXX) $(CXX_FLAGS) $(OPT) $(EXE).o -o $(EXE)_debug

# Remove executable binary and generated objected files.
.PHONY : clean
clean : 
	rm -f $(EXE) $(EXE)_debug *.o