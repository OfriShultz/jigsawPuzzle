#include "Piece.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

#ifndef PUZZLE_H_
#define PUZZLE_H_

//pieces values postion on array
#define ID 		0
#define LEFT	1
#define TOP 	2
#define RIGHT	3
#define BOTTOM	4

#define NUM_OF_CORNERS 4

class Puzzle {

	int numOfPieces = 0;
	Piece** pieces = nullptr;
	vector<vector<int> > solution;
	int missingCorners[2 * NUM_OF_CORNERS] = { -1, -1, -1, -1, -1, -1, -1, -1 };

public:
	Puzzle() {
	}
	;
	~Puzzle() {
		for (int i = 0; i < numOfPieces; i++)
			delete pieces[i];
		delete[] pieces;
	}
	;
	//initialize puzzle by numOfElements
	void initPuzzle(int numOfElements);
	//numOfElements getter
	int getNumOfPieces() const;
	//piece at index getter
	const Piece* getPieceAtIndex(int i) const;
	//piece at index setter (by values in int array)
	bool setPieceAtIndex(int piecevalues[]);
	//checks if B could fit after A in a row
	bool fitBLeftToA(Piece* a, Piece* b);
	//checks if B could fit below A in a col
	bool fitBUnderA(Piece* a, Piece* b);
	//checks a piece fits the next free spot in the solution according to the rules
	bool fit(Piece* p, vector<vector<int> > &sol, int piecesLeft, int width);
	//solve puzzle envelope function calls SolvePuzzleRec
	bool SolvePuzzle();
	//solving the puzzle in brute force attempts
	bool SolvePuzzleRec(vector<vector<int> > sol, bool used[], int piecesLeft, unsigned int width);
	//returns the array of missing corners
	int* getMissingCorners();
	//outputs the solution to the file, if found
	void solutionOutput(ofstream &outFile);
};

#endif /* PUZZLE_H_ */
