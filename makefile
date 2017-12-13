COMP = g++
#put all your object files here
OBJS = main.o Puzzle.o puzzleUtil.o 
#The executabel filename DON'T CHANGE
EXEC = ex1
CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
Puzzle.o: Puzzle.cpp Piece.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
puzzleUtil.o: puzzleUtil.cpp Puzzle.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
main.o: main.cpp puzzleUtil.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)