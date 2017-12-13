#include "PieceTree.h"
#include <iostream>

PieceTree::PieceTree() {
	root_left = make_shared<PieceNode>();
	initTree(root_left, 0);
}

void PieceTree::rotatepiece(int values[], int factor) {
	if (factor < 0)
		factor += 4 * ROTATE_DEGREE;
	for (int k = 0; k < factor; k += ROTATE_DEGREE) {
		int temp = values[PIECE_NUM_CORNERS - 1];
		for (int i = PIECE_NUM_CORNERS - 1; i > 0; i--) {
			values[i] = values[i - 1];
		}
		values[0] = temp;
	}
}

void PieceTree::insertPiece(shared_ptr<Piece> &piece, bool rotate) {
	int piecevals[PIECE_NUM_CORNERS] = { piece->getLeft() + 1, piece->getTop() + 1, piece->getRight() + 1, piece->getBottom() + 1 };
	if (rotate) {
		for (int i = 0; i < PIECE_NUM_CORNERS; i++) {
			insertPiece(root_left, piece, 0, ROTATE_DEGREE * i, piecevals);
			rotatepiece(piecevals);
		}
	} else
		insertPiece(root_left, piece, 0, 0, piecevals);
}

void PieceTree::insertPiece(shared_ptr<PieceNode> &root, shared_ptr<Piece> &piece, int depth, int angle, int piecevals[]) {
	if (depth == 3) {
		root->nodepieces.at(piecevals[depth]).push_back(make_pair(piece, angle));
		root->nodepieces.at(DONTCARE).push_back(make_pair(piece, angle));
		return;
	}
	insertPiece(root->children.at(piecevals[depth]), piece, depth + 1, angle, piecevals);
	insertPiece(root->children.at(DONTCARE), piece, depth + 1, angle, piecevals);
}

void PieceTree::initTree(shared_ptr<PieceNode> &root, int depth) {
	for (int i = 0; i < MAX_DEPTH; i++) {
		if (depth < MAX_DEPTH - 1) {
			shared_ptr<PieceNode> node = make_shared<PieceNode>();
			root->children.push_back(move(node));
			initTree(root->children.back(), depth + 1);
		} else {
			root->nodepieces.push_back(vector<pair<shared_ptr<Piece>, short>>());
		}
	}
}

vector<pair<shared_ptr<Piece>, short>>& PieceTree::getProperPieces(int arr[]) {
	return root_left->children.at(arr[LEFT])->children.at(arr[TOP])->children.at(arr[RIGHT])->nodepieces.at(arr[BOTTOM]);
}

