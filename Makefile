# Personal Makefile Template.
CXX = g++ -std=c++17
CXX_FLAGS = -Wall -Werror -Wextra -pedantic -Wconversion
OPT = -O3 -DNDEBUG
DEBUG = -g3 -DDEBUG

HEAD = circular_buffer
EXE = test

# Build optimized version.
release : $(HEAD).h
	$(CXX) $(CXX_FLAGS) $(OPT) $(EXE).cpp -o $(EXE)

# Build debugging version.
debug : $(HEAD).h
	$(CXX) $(CXX_FLAGS) $(OPT) $(EXE).cpp -o $(EXE)_debug

# Remove executable binary and generated objected files.
.PHONY : clean
clean : 
	rm -f $(EXE) $(EXE)_debug *.o
