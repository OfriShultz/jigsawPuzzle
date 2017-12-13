#ifndef PIECE_H_
#define PIECE_H_

#define PIECE_NUM_FIELDS 5

class Piece {
	int id;
	int left;
	int top;
	int right;
	int bottom;
public:
	Piece(int id, int left, int top, int right, int bottom) :
			id(id), left(left), top(top), right(right), bottom(bottom) {
	}
	Piece() :
			Piece(-1, -1, -1, -1, -1) {
	}
	~Piece() {
	}
	;

	int getBottom() const {
		return bottom;
	}

	int getId() const {
		return id;
	}

	int getLeft() const {
		return left;
	}

	int getRight() const {
		return right;
	}

	int getTop() const {
		return top;
	}

};
#endif /* PIECE_H_ */
