#include <ctime>
#include "puzzleUtils.h"

#include "tester.hpp"



//todo invalid format prints corenrs missing

int main(int argc, char * argv[]) {
	int start_s = clock();
	ifstream inFile;
	ofstream outFile;
	bool rotate;
	if(!puzzleUtils::checkArgsAndFiles(argc,argv,inFile,outFile,rotate))
		return -1; //if any error occurred - exit

	//---------------------------PREPROCCESS PUZZLE--------------------------
	Puzzle puzzle;
	puzzle.setRotate(rotate);
	//preproccess puzzle - if any error - dont try to solve
	if (puzzleUtils::preprocessPuzzle(puzzle, inFile, outFile)) {
		outFile.close(); //clears the output file
		outFile.open(argv[2]);
		if (!outFile) {
			cerr << "ERROR: failed to open output file";
			return -1;
		}

	//------------------------------SOLVE PUZZLE-----------------------------
		//try to solve puzzle - if cannot print error
		if (!puzzle.SolvePuzzle()) {
			puzzleUtils::ErrorCannotSolve(outFile); //no solution found
		} else
			puzzle.solutionOutput(outFile); //solution found - print it
	}
	outFile.close(); //infile is closed after reading from file

	int stop_s = clock();
	cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << endl;
	return 0;
}
