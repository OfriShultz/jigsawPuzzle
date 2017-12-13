# jigsawPuzzle

the program gets an input file which represents the different parts in the jigsaw puzzle - the format of the input file would be explained below.
The program will find the solution for the puzzle and create an output file representing the solution.

# Input File:
The input file is a text file in the following format:
The first line consists the following information:
NumElements=24
and then list of all the pieces with id in range [1...NumElements].
each line of piece description will consist of the jigsaw element ID as the first number.
Then come 4 numbers representing the 4 sides of the element: left, top, right, bottom.
0 means that the edge at this side is straight.
1 means that the edge at this side is exterior (a “male”).
-1 means that the edge at this side is interior (a “female”).

# Output File
The output file will consist the IDs of the elements in the structure that solves the puzzle:
A single space between IDs on the same line, a new line for each row in the puzzle.
