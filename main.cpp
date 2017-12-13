#include "puzzleUtil.h"

int main(int argc, char *argv[]) {
	//---------------------------OPEN FILES---------------------------
	if (argc != 3) {
		cerr << "ERROR: invalid number of arguments";
		return -1;
	}
	// open inFile and outFile
	ifstream inFile(argv[1]);
	ofstream outFile(argv[2]);
	if (!outFile) {
		cerr << "ERROR: failed to open output file";
		return -1;
	}
	if (!inFile) {
		cerr << "ERROR: failed to open input file";
		outFile.close();
		return -1;
	}
	//---------------------------SOLVE PUZZLE--------------------------
	Puzzle puzzle;
	//preproccess puzzle - if any error - dont try to solve
	if (puzzleUtil::preprocessPuzzle(puzzle, inFile, outFile)) {
		outFile.close(); //clears the output file
		outFile.open(argv[2]);
		if (!outFile) {
			cerr << "ERROR: failed to open output file";
			return -1;
		}
		//try to solve puzzle - if cannot print error
		if (!puzzle.SolvePuzzle()) {
				puzzleUtil::ErrorCannotSolve(outFile);
		} else
			puzzle.solutionOutput(outFile);
	}
	outFile.close();
	return 0;
}
