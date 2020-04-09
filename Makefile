# Personal Makefile Template.
CXX = g++ -std=c++17
CXX_FLAGS = -Wall -Werror -Wextra -Wconversion -pedantic -Wfloat-equal -Wduplicated-branches -Wduplicated-cond -Wshadow -Wdouble-promotion -Wundef
OPT = -O3 -DNDEBUG
DEBUG = -g3 -DDEBUG

HEAD = circular_buffer
EXE = test

# Build optimized version.
release : $(HEAD).h
	$(CXX) $(CXX_FLAGS) $(OPT) $(EXE).cpp -o $(EXE)

# Build debugging version.
debug : $(HEAD).h
	$(CXX) $(CXX_FLAGS) $(OPT) $(EXE).cpp -o $(EXE)

# Remove executable binary and generated objected files.
.PHONY : clean
clean : 
	rm -f $(EXE)
