#include "PieceTree.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h> //todo ask if we should use c functions

#ifndef PUZZLE_H_
#define PUZZLE_H_

#define NUM_OF_CORNERS 4

class Puzzle {

	int numOfPieces = 0, factor = 0;
	vector<vector<pair<shared_ptr<Piece>, short>>> sol;
	unique_ptr<PieceTree> piecetree = make_unique<PieceTree>();
	bool rotate = false, oneColSol=true, oneRowSol=true;


public:
	Puzzle() {
	}
	;
	~Puzzle() {
	}
	;
	//initialize puzzle by numOfElements
	void setNumOfPieces(int numOfElements);
	//numOfElements getter
	int getNumOfPieces() const;
	//solve puzzle envelope function calls SolvePuzzleRec
	bool SolvePuzzle();
	//solving the puzzle in brute force attempts
	bool SolvePuzzleTree(int piecesLeft, unsigned int width,int factor=0);
	//outputs the solution to the file, if found
	void solutionOutput(ofstream &outFile);

	void setWantedValues(int left, int top,int right, int bottom,int wantedvalues[]);

	vector<pair<shared_ptr<Piece>, short>>& getPiecesFromTree(int piecesLeft, int width,int factor);

	bool setPieceAtTree(int piecevalues[], bool created[], shared_ptr<Piece> &piece);

	void initSquareBoolArray(bool badpieces[][PIECE_DIFFERENT_VALUES], bool value, int size);

	bool isBadPiece(pair<shared_ptr<Piece>, short> &p, bool badpieces[][PIECE_DIFFERENT_VALUES],int factor);

	void markBadPiece(pair<shared_ptr<Piece>, short> &p, bool badpieces[][PIECE_DIFFERENT_VALUES],int factor);

	void setRotate(bool rotate) {
		this->rotate = rotate;
	}
	bool isRotate() {
		return rotate;
	}

	bool isValidSolution();

	void setOneRowColSolution(bool oneRowSol,bool oneColSol) {
		this->oneRowSol = oneRowSol;
		this->oneColSol = oneColSol;
	}
};

#endif /* PUZZLE_H_ */
