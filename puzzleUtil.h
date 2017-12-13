#include "Puzzle.h"
#include <unordered_set>

#ifndef PUZZLEUTIL_H_
#define PUZZLEUTIL_H_

//corner macros
#define TL 0 //top left
#define TR 1 //top right
#define BL 2 //bottom left
#define BR 3 //bottom right
#define TLBL 4 //topleftbottomleft
#define TRBR 5 //toprightbottomright
#define TLTR 6 //toplefttopright
#define BLBR 7 //bottomleftbottomright
#define LINE_LENGTH 5

class puzzleUtil {
public:
	//checks if a string is int
	static bool isInt(string &s);
	//removes spaces from a string
	static string removespaces(const string &s);
	//split a string to a vector byb delimiter
	static vector<string> split(const string &s, char delim = ' ');
	//checks if a piece is square
	static bool checkSquare(const Piece* p);
	//checks if an input line is in valid format
	static bool CheckLineFormat(int pvals[], vector<string> &splt);
	//preprocess the puzzle - reads from file and check for presolution errors
	static bool preprocessPuzzle(Puzzle &puzzle, ifstream &inFile, ofstream &outFile);

	//---------------------------ERROR HANDELING---------------------------
	//marks the numbers of horizontal and vertical edges (separately)
	static void MarkEdges(const Piece *p, int &horizontal_edges, int &vertical_edges);
	//marks the corners by type (pushes them into a corresponding vector)
	static void MarkCorners(const Piece* p, vector<int> corners[]);
	//checks if there a different corner type for each type of solution
	static void CheckCorners(vector<int> corners[], int res[]);
	//checks missing corners - if so : error
	static bool ErrorMissingCorners(ofstream &outFile, int* res);
	//checks number of straight edges - if so : error
	static bool ErrorWrongStraightEdges(ofstream &outFile);
	//checks if format is wrong - if so : error
	static bool ErrorWrongFormat(ofstream &outFile, vector<pair<string, string> > &wrongFormat);
	//checks wrong id - if so : error
	static bool ErrorWrongID(const Puzzle &puzzle, ofstream &outFile, vector<int> &wrongID);
	//checks non digit id - if so : error
	static bool ErrorNonDigitID(ofstream &outFile, vector<string> &nonDigitID);
	//checks duplicates id - if so : error
	static bool ErrorDuplicates(ofstream &outFile, vector<int> &duplicates);
	//checks missing elements - if so : error
	static bool ErrorMissingElements(const Puzzle &puzzle, ofstream &outFile, int numOfMissing);
	//checks sum of edges is not zero - if so : error
	static bool ErrorSumOfEdges(ofstream &outFile);
	//outputs to file that puzzle cannor be solved
	static bool ErrorCannotSolve(ofstream &outFile);

};

#endif /* PUZZLEPREP_H_ */
