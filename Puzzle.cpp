#include "Puzzle.h"

void Puzzle::initPuzzle(int numOfPieces) {
	if (this->numOfPieces == 0) { //init only if uninitialized
		this->numOfPieces = numOfPieces;
		pieces = new Piece*[numOfPieces]();
	}
}

int Puzzle::getNumOfPieces() const {
	return numOfPieces;
}

bool Puzzle::setPieceAtIndex(int piecevalues[]) {
	if (pieces[piecevalues[ID] - 1] == nullptr) { //if not set
		pieces[piecevalues[ID] - 1] = new Piece(piecevalues[ID] - 1, piecevalues[LEFT], piecevalues[TOP], piecevalues[RIGHT], piecevalues[BOTTOM]);
		return true;
	}
	return false;
}

int* Puzzle::getMissingCorners() {
	return missingCorners;
}

const Piece* Puzzle::getPieceAtIndex(int i) const {
	return pieces[i];
}

bool Puzzle::SolvePuzzle() {
	bool* used = new bool[numOfPieces](); //array for pieces being used in solution
	vector<vector<int> > sol(0);
	bool solved = SolvePuzzleRec(sol, used, numOfPieces, 0);
	delete[] used;
	return solved;
}

bool Puzzle::SolvePuzzleRec(vector<vector<int> > sol, bool used[], int piecesLeft, unsigned int width) {
	if (piecesLeft == 0) { //halt cond = no more pieces available
		solution = sol;
		return true;
	}
	//no line or line end - add a new one
	if (sol.size() == 0 || sol.back().size() == width)
		sol.push_back(vector<int>());
	for (int i = 0; i < numOfPieces; i++) {
		//if  a piece is unused and it  fits the puzzle
		if (!used[i] && fit(pieces[i], sol, piecesLeft, width)) {
			used[i] = true;
			sol.back().push_back(pieces[i]->getId());
			//if first line of puzzle and the current piece has a flat right
			//check if the current width is possible - if so recurse with current width
			if (sol.size() == 1 && pieces[i]->getRight() == 0 && numOfPieces % sol.at(0).size() == 0) {
				if (SolvePuzzleRec(sol, used, piecesLeft - 1, sol.at(0).size()))
					return true;
			}
			//recure on same line to find next piece
			if (SolvePuzzleRec(sol, used, piecesLeft - 1, width))
				return true;
			used[i] = false;
			sol.back().pop_back();
		}
	}
	return false;
}

void Puzzle::solutionOutput(ofstream &outFile) {
	for (unsigned int i = 0; i < solution.size(); i++) {
		for (unsigned int j = 0; j < solution[i].size(); j++) {
			outFile << solution[i][j] + 1;
			if (j != solution[i].size() - 1)
				outFile << " ";
		}
		outFile << "\n";
	}
}

bool Puzzle::fit(Piece* b, vector<vector<int> > &sol, int piecesLeft, int width) {
	Piece* leftpiece = nullptr;
	Piece* abovepiece = nullptr;
	int lastrow = sol.size() - 1; 		 //last used row idx of sol 2d vector
	int lastcol = sol.back().size() - 1; //last used col idx of sol 2d vector
	//first in row and doesnt have a flat left
	if (lastcol == -1 && b->getLeft() != 0)
		return false;
	//first in col and doesnt have a flat top
	if (lastrow == 0 && b->getTop() != 0)
		return false;
	//last in row and doesnt have a flat right
	if (lastcol == width - 2 && b->getRight() != 0)
		return false;
	//last in col and doesnt have a flat bottom
	if (piecesLeft <= width && b->getBottom() != 0)
		return false;
	if (lastcol != -1) //col isnt empty
		leftpiece = pieces[sol.at(lastrow).at(lastcol)];
	if (lastrow != 0) //row isnt empty
		abovepiece = pieces[sol.at(lastrow - 1).at(lastcol + 1)];
	if (!fitBLeftToA(leftpiece, b))  //fit to sides
		return false;
	if (!fitBUnderA(abovepiece, b)) //fit to top
		return false;
	return true;
}

bool Puzzle::fitBLeftToA(Piece* a, Piece* b) {
	if (a == nullptr)
		return true;
	return (a->getRight() + b->getLeft()) == 0;
}

bool Puzzle::fitBUnderA(Piece* a, Piece* b) {
	if (a == nullptr)
		return true;
	return (a->getBottom() + b->getTop()) == 0;
}

