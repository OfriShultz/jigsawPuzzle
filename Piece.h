#ifndef PIECE_H_
#define PIECE_H_

#include <vector>
#include <memory>
#include <utility>
#include <iostream>
using namespace std;

//pieces values postion on array
#define ID 0
#define LEFT	0
#define TOP 	1
#define RIGHT	2
#define BOTTOM	3

#define PIECE_DIFFERENT_VALUES 3
#define PIECE_NUM_FIELDS 5
#define PIECE_NUM_CORNERS 4
#define ROTATE_DEGREE 90
#define ROTATE_MAX 360
#define ROTATE_INDEX(old,rotate) old-rotate >= 0 ? old-rotate : 4+(old-rotate)

class Piece {
	int id;
	int piecevalues[4];
	bool used;

public:
	Piece(int id, int left, int top, int right, int bottom) :
			id(id), used(false) {
		piecevalues[LEFT] = left;
		piecevalues[TOP] = top;
		piecevalues[RIGHT] = right;
		piecevalues[BOTTOM] = bottom;
	}
	Piece() :
			Piece(-1, -1, -1, -1, -1) {
	}
	~Piece() {
	}
	;

	int getId() const {
		return id;
	}

	int getLeft(short angle = 0) const { //todo function of angles and rotations for fucks sake
		angle = angle < 0 ? angle+4*ROTATE_DEGREE : angle;
		return piecevalues[ROTATE_INDEX(LEFT, (angle / ROTATE_DEGREE) % 4)];
	}

	int getBottom(short angle = 0) const {
		angle = angle < 0 ? angle+4*ROTATE_DEGREE : angle;
		return piecevalues[ROTATE_INDEX(BOTTOM, (angle / ROTATE_DEGREE) % 4)];
	}

	int getRight(short angle = 0) const {
		angle = angle < 0 ? angle+4*ROTATE_DEGREE : angle;
		return piecevalues[ROTATE_INDEX(RIGHT, (angle / ROTATE_DEGREE) % 4)];
	}

	int getTop(short angle = 0) const {
		angle = angle < 0 ? angle+4*ROTATE_DEGREE : angle;
		return piecevalues[ROTATE_INDEX(TOP, (angle / ROTATE_DEGREE) % 4)];
	}
	int isUsed() const {
		return used;
	}
	void setUsed(bool status) {
		used = status;
	}

};
#endif /* PIECE_H_ */
