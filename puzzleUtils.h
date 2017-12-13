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

class puzzleUtils {

	// puzzleParamaters - a struct to encapsulate all relevant preproccessed data of a puzzle
	// used for error handleing and solution optimization
	struct puzzleParamaters {
		int sumOfEdges = 0, numOfPieces = 0, pieceCount = 0, edges[NUM_OF_CORNERS];
		bool oneRowSol = true, oneColSol = true, *created;
		vector<int> wrongID, duplicates, corners[2 * NUM_OF_CORNERS];
		vector<string> splittedLine, nonDigitID;
		vector<pair<string, string> > wrongFormat;
	};

public:
	//checks the command line arguments and open input and output files
	static bool checkArgsAndFiles(int argc, char *argv[], ifstream &in, ofstream &out, bool &rotate);
	//checks if a string is int
	static bool isInt(string &s);
	//removes spaces from a string
	static string removespaces(const string &s);
	//split a string to a vector byb delimiter
	static vector<string> split(const string &s, char delim = ' ');
	//checks if a piece is square
	static bool checkSingleSquare(vector<int> corners[]);
	//checks if an input line is in valid format
	static bool CheckLineFormat(int pvals[], vector<string> &splt);
	//preprocess the puzzle - reads from file and check for presolution errors
	static bool errorHandling(Puzzle &puzzle, ofstream &outFile, puzzleParamaters &data);

	static int readNumOfPiecesFromFile(ifstream &inFile, ofstream &outFile);

	static void readPiecesFromFile(Puzzle &puzzle, ifstream &inFile, puzzleParamaters &data);

	static bool preprocessPuzzle(Puzzle &puzzle, ifstream &inFile, ofstream &outFile);

	static bool markOneRowSolution(bool rotate, shared_ptr<Piece>& p);

	static bool markOneColSolution(bool rotate, shared_ptr<Piece>& p);

	//---------------------------ERROR HANDELING---------------------------
	//marks the numbers of flat edges
	static void MarkEdges(shared_ptr<Piece>& p, int edges[]);
	//marks the corners by type (pushes them into a corresponding vector)
	static void MarkCorners(shared_ptr<Piece>& p, vector<int> corners[], bool rotate); //TODO CONST
	//checks if there a different corner type for each type of solution
	static void CheckCorners(vector<int> corners[], int res[]);
	//checks missing corners - if so : error
	static bool checkMissingCorners(ofstream &out, vector<int> corners[], int numPieces, bool &oneRowSol, bool &oneColSol);
	//checks number of straight edges - if so : error
	static bool checkWrongStraightEdges(ofstream &outFile, int numOfPieces, int edges[], bool rotate);
	//checks if format is wrong - if so : error
	static bool checkWrongFormat(ofstream &outFile, vector<pair<string, string> > &wrongFormat);
	//checks wrong id - if so : error
	static bool checkWrongID(const Puzzle &puzzle, ofstream &outFile, vector<int> &wrongID);
	//checks non digit id - if so : error
	static bool checkNonDigitID(ofstream &outFile, vector<string> &nonDigitID);
	//checks duplicates id - if so : error
	static bool checkDuplicates(ofstream &outFile, vector<int> &duplicates);
	//checks missing elements - if so : error
	static bool checkMissingElements(const Puzzle &puzzle, ofstream &outFile, int numOfPieces, int pieceCount,
			bool created[]);
	//checks sum of edges is not zero - if so : error
	static bool checkSumOfEdges(ofstream &outFile, int sumOfEdges);
	//outputs to file that puzzle cannor be solved
	static bool ErrorCannotSolve(ofstream &outFile);

};

#endif /* PUZZLEPREP_H_ */
