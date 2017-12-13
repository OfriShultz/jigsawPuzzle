#ifndef PIECETREE_H_
#define PIECETREE_H_
#define DONTCARE 3

#define TAB_IN 0
#define TAB_FLAT 1
#define TAB_OUT 2
#define MAX_DEPTH 4

#include "Piece.h"

class PieceTree {

private:
	class PieceNode {
		friend class PieceTree;
		vector<vector<pair<shared_ptr<Piece>, short>>> nodepieces;
		vector<shared_ptr<PieceNode> > children;
	};

	shared_ptr<PieceNode> root_left;

	void initTree(shared_ptr<PieceNode> &root, int depth);

public:
	PieceTree();
	~PieceTree() {
	}
	;

	void rotatepiece(int values[], int factor = ROTATE_DEGREE); //move to utils and renmae

	void insertPiece(shared_ptr<Piece> &piece, bool rotate);

	void insertPiece(shared_ptr<PieceNode> &root, shared_ptr<Piece> &piece, int depth, int angle, int piecevals[]);

	vector<pair<shared_ptr<Piece>, short>>& getProperPieces(int arr[]);

};

#endif /* PIECETREE_H_ */
