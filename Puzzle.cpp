#include "Puzzle.h"

void Puzzle::setNumOfPieces(int numOfPieces) {
	if (this->numOfPieces == 0) { //init only if uninitialized
		this->numOfPieces = numOfPieces;
	}
}

int Puzzle::getNumOfPieces() const {
	return numOfPieces;
}

bool Puzzle::setPieceAtTree(int piecevalues[], bool created[], shared_ptr<Piece> &piece) {
	if (created[piecevalues[ID] - 1])
		return false;
	piece = make_shared<Piece>(Piece(piecevalues[ID], piecevalues[LEFT + 1],     //create shared piece
			piecevalues[TOP + 1], piecevalues[RIGHT + 1], piecevalues[BOTTOM + 1]));
	piecetree->insertPiece(piece, rotate);
	created[piecevalues[ID] - 1] = true;
	return true;
}

bool Puzzle::SolvePuzzle() {
	int factor = 270;
	if (factor == 90 || factor == 270)
		swap(oneRowSol, oneColSol);
	if (oneRowSol)
		return SolvePuzzleTree(numOfPieces, numOfPieces, factor);
	if (oneColSol)
		return SolvePuzzleTree(numOfPieces, 1, factor);
	return SolvePuzzleTree(numOfPieces, 0, factor);
}

void Puzzle::initSquareBoolArray(bool badpieces[][PIECE_DIFFERENT_VALUES], bool value, int size) {
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			badpieces[i][j] = value;
}

bool Puzzle::isBadPiece(pair<shared_ptr<Piece>, short> &p, bool badpieces[][PIECE_DIFFERENT_VALUES],int factor) {
	int right = p.first->getRight(p.second + factor);
	int bottom = p.first->getBottom(p.second + factor);
	return badpieces[right + 1][bottom + 1];
}

void Puzzle::markBadPiece(pair<shared_ptr<Piece>, short> &p, bool badpieces[][PIECE_DIFFERENT_VALUES], int factor) {
	int right = p.first->getRight(p.second + factor);
	int bottom = p.first->getBottom(p.second + factor);
	badpieces[right + 1][bottom + 1] = true;
}

bool Puzzle::SolvePuzzleTree(int piecesLeft, unsigned int width, int factor) {
	if ((!oneRowSol && piecesLeft < numOfPieces / 2 && width == 0) || (!oneColSol && width == 1))
		return false;
	if (piecesLeft == 0) { //halt cond = no more pieces available
		this->factor = factor;
		return true;
	}
	bool badpieces[PIECE_DIFFERENT_VALUES][PIECE_DIFFERENT_VALUES];
	initSquareBoolArray(badpieces, false, PIECE_DIFFERENT_VALUES);
	//no line or line end - add a new one
	if (sol.size() == 0 || sol.back().size() == width)
		sol.push_back(vector<pair<shared_ptr<Piece>, short>>());
	for (auto pair : getPiecesFromTree(piecesLeft, width, factor)) {
		if (!pair.first->isUsed() && !isBadPiece(pair, badpieces,factor)) {
			pair.first->setUsed(true);
			sol.back().push_back(pair);
			if (sol.size() == 1 && pair.first->getRight(pair.second + factor) == 0 && !oneRowSol && numOfPieces % sol.at(0).size() == 0) {
				if (SolvePuzzleTree(piecesLeft - 1, sol.at(0).size(), factor))
					return true;
			}
			if (SolvePuzzleTree(piecesLeft - 1, width, factor))
				return true;
			pair.first->setUsed(false);
			markBadPiece(pair, badpieces, factor);
			sol.back().pop_back();
		}
	}
	if (sol.back().size() == 0)
		sol.pop_back();
	return false;
}

bool Puzzle::isValidSolution() {
	for (unsigned int i = 0; i < sol.size(); i++) {
		for (unsigned int j = 0; j < sol.at(i).size(); j++) {
			int top = i > 0 ? sol.at(i - 1).at(j).first->getBottom(sol.at(i - 1).at(j).second + factor) : 0;
			int left = j > 0 ? sol.at(i).at(j - 1).first->getRight(sol.at(i).at(j - 1).second + factor) : 0;
			int right = j < sol.at(i).size() - 1 ? sol.at(i).at(j + 1).first->getLeft(sol.at(i).at(j + 1).second + factor) : 0;
			int bottom = i < sol.size() - 1 ? sol.at(i + 1).at(j).first->getTop(sol.at(i + 1).at(j).second + factor) : 0;
			pair<shared_ptr<Piece>, short> p = sol.at(i).at(j);
			if (p.first->getLeft(p.second + factor) + left != 0 || p.first->getRight(p.second + factor) + right != 0
					|| p.first->getBottom(p.second + factor) + bottom != 0 || p.first->getTop(p.second + factor) + top != 0) {
				cerr << " soultion invalid";
				return false;
			}
		}
	}
	cout << " soultion is valid";
	return true;
}

void Puzzle::solutionOutput(ofstream &outFile) {
	switch (factor) {
	case 90:
		for (int i = sol.at(0).size() - 1; i >= 0; i--) {
			for (int j = 0; j < (int) sol.size(); j++) {
				outFile << sol.at(j).at(i).first->getId();
				if (sol.at(j).at(i).second)
					outFile << " [" << sol.at(j).at(i).second << "] ";
				if (j != (int) sol.size() - 1)
					outFile << " ";
			}
			outFile << "\n";
		}
		break;
	case 180:
		for (int i = sol.size() - 1; i >= 0; i--) {
			for (int j = sol.at(0).size() - 1; j >= 0; j--) {
				outFile << sol.at(i).at(j).first->getId();
				if (sol.at(i).at(j).second)
					outFile << " [" << sol.at(i).at(j).second << "] ";
				if (j != 0)
					outFile << " ";
			}
			outFile << "\n";
		}
		break;
	case 270:
		for (int i = 0; i < (int) sol.at(0).size(); i++) {
			for (int j = sol.size() - 1; j >= 0; j--) {
				outFile << sol.at(j).at(i).first->getId();
				if (sol.at(j).at(i).second)
					outFile << " [" << sol.at(j).at(i).second << "] ";
				if (j != 0)
					outFile << " ";
			}
			outFile << "\n";
		}
		break;
	default:
		for (unsigned int i = 0; i < sol.size(); i++) {
			for (unsigned int j = 0; j < sol.at(i).size(); j++) {
				outFile << sol.at(i).at(j).first->getId();
				if (sol.at(i).at(j).second)
					outFile << " [" << sol.at(i).at(j).second << "] ";
				if (j != sol.at(i).size() - 1)
					outFile << " ";
			}
			outFile << "\n";
		}
	}
}

void Puzzle::setWantedValues(int left, int top, int right, int bottom, int wantedvalues[]) {
	wantedvalues[LEFT] = left;
	wantedvalues[TOP] = top;
	wantedvalues[RIGHT] = right;
	wantedvalues[BOTTOM] = bottom;
}

vector<pair<shared_ptr<Piece>, short>>& Puzzle::getPiecesFromTree(int piecesLeft, int width, int factor) {
	int lastrow = sol.size() - 1; 		 //last used row idx of sol 2d vector
	int lastcol = sol.back().size() - 1; 		 //last used col idx of sol 2d vector
	int leftpiece = DONTCARE;
	int abovepiece = DONTCARE;
	int wantedvalues[NUM_OF_CORNERS] = { DONTCARE, DONTCARE, DONTCARE, DONTCARE };
	if (lastcol != -1)
		leftpiece = (-1 * (sol.at(lastrow).at(lastcol).first->getRight(sol.at(lastrow).at(lastcol).second + factor)) + 1);
	if (lastrow != 0)
		abovepiece = (-1 * (sol.at(lastrow - 1).at(lastcol + 1).first->getBottom(sol.at(lastrow - 1).at(lastcol + 1).second + factor)) + 1);
	if (lastrow == 0) { //first row
		if (lastcol == -1) { //first in col
			if (width == 1) //last in col
				setWantedValues(TAB_FLAT, TAB_FLAT, TAB_FLAT, DONTCARE, wantedvalues);
			else if (width == numOfPieces) //last in col
				setWantedValues(TAB_FLAT, TAB_FLAT, DONTCARE, TAB_FLAT, wantedvalues);
			else
				setWantedValues(TAB_FLAT, TAB_FLAT, DONTCARE, DONTCARE, wantedvalues);
		} else if (lastcol != width - 2) { //mid in col
			if (width == numOfPieces)
				setWantedValues(leftpiece, TAB_FLAT, DONTCARE, TAB_FLAT, wantedvalues);
			else
				setWantedValues(leftpiece, TAB_FLAT, DONTCARE, DONTCARE, wantedvalues);
		} else { //last in col
			if (width == numOfPieces)
				setWantedValues(leftpiece, TAB_FLAT, TAB_FLAT, TAB_FLAT, wantedvalues);
			else
				setWantedValues(leftpiece, TAB_FLAT, TAB_FLAT, DONTCARE, wantedvalues);
		}
	} else if (piecesLeft <= width) { //last row
		if (lastcol == -1) { //first in col
			if (width == 1) //first and last
				setWantedValues(TAB_FLAT, abovepiece, TAB_FLAT, TAB_FLAT, wantedvalues);
			else
				setWantedValues(TAB_FLAT, abovepiece, DONTCARE, TAB_FLAT, wantedvalues);
		} else if (lastcol != width - 2) //mid in col
			setWantedValues(leftpiece, abovepiece, DONTCARE, TAB_FLAT, wantedvalues);
		else { //last in col
			setWantedValues(leftpiece, abovepiece, TAB_FLAT, TAB_FLAT, wantedvalues);
		}
	} else { //mid row
		if (lastcol == -1) { //first in col
			if (width == 1) //first and last
				setWantedValues(TAB_FLAT, abovepiece, TAB_FLAT, DONTCARE, wantedvalues);
			else
				setWantedValues(TAB_FLAT, abovepiece, DONTCARE, DONTCARE, wantedvalues);
		} else if (lastcol != width - 2) //mid in col
			setWantedValues(leftpiece, abovepiece, DONTCARE, DONTCARE, wantedvalues);
		else {  //last in col
			setWantedValues(leftpiece, abovepiece, TAB_FLAT, DONTCARE, wantedvalues);
		}
	}
	piecetree->rotatepiece(wantedvalues, -factor);
	return piecetree->getProperPieces(wantedvalues);
}

