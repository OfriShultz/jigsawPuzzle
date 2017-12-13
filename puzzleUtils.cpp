#include "puzzleUtils.h"

bool puzzleUtils::checkArgsAndFiles(int argc, char *argv[], ifstream &in, ofstream &out, bool &rotate) {
	int rotateIndex = -1, inIndex = 1, outIndex = 2;
	//---------------------------OPEN FILES---------------------------
	if (argc != 3 && argc != 4) {
		cerr << "ERROR: invalid number of arguments";
		return false;
	} //check for -rotate
	if (argc == 4) {
		for (int i = 0; i < argc; i++)
			if (!strcmp("-rotate", argv[i]))
				rotateIndex = i;
		if (rotateIndex == -1) {
			cerr << "ERROR: invalid number of arguments";
			return false;
		}
		if (rotateIndex != 3) {
			inIndex = rotateIndex == 1 ? 2 : 1;
			outIndex = 3;
		}
	}
	rotate = rotateIndex != -1;
	// open inFile and outFile
	in.open(argv[inIndex]);
	if (!in) {
		cerr << "ERROR: failed to open input file";
		return false;
	}
	out.open(argv[outIndex]);
	if (!out) {
		cerr << "ERROR: failed to open output file";
		in.close();
		return false;
	}
	return true;
}

bool puzzleUtils::isInt(string &s) {
	for (size_t n = 0; n < s.length(); n++) {
		if (n == 0 && s[n] == '-') //number could be negertive
			continue;
		if (!isdigit(s[n]))
			return false;
	}
	return true;
}

string puzzleUtils::removespaces(const string &s) {
	stringstream strStrm(s);
	string item, item2;
	while (getline(strStrm, item, ' ')) {
		if (item.empty())
			continue;
		item2 += item;
		if (isInt(item))
			break;
	}
	return item2;
}

vector<string> puzzleUtils::split(const string &s, char delim) {
	vector<string> elements;
	stringstream strStrm(s);
	string item;
	while (getline(strStrm, item, delim)) {
		if (item.empty())
			continue;
		elements.push_back(item);
	}
	return elements;
}

bool puzzleUtils::CheckLineFormat(int pvals[], vector<string> &splt) {
	if (splt.empty())
		return false;
	bool formatok = true;
	if (splt.size() != LINE_LENGTH) {
		return false;
	}
	for (int i = 0; i < LINE_LENGTH; i++) {
		if (isInt(splt.at(i))) {
			int temp = stoi(splt.at(i));
			// id > 0 and coordinates are [-1,1]
			if ((i != 0 && temp >= -1 && temp <= 1) || i == 0)
				pvals[i] = temp;
			else {
				formatok = false;
				break;
			}
		} else if (i != 0) {
			formatok = false;
			break;
		}
	}
	return formatok;
}
int puzzleUtils::readNumOfPiecesFromFile(ifstream &inFile, ofstream &outFile) {
	string line;
	vector<string> splittedLine;
	//get first line for number of elements
	while (line.empty())
		getline(inFile, line);
	splittedLine = split(removespaces(line), '='); //removes spacs and equal sign
	if (splittedLine.size() < 2 || splittedLine.at(0).compare("NumElements") != 0 || !isInt(splittedLine.at(1))) {
		outFile << "ERROR: invalid input file format" << endl;
		return false;
	}
	return stoi(splittedLine.at(1));
}

void puzzleUtils::readPiecesFromFile(Puzzle &puzzle, ifstream &inFile, puzzleParamaters &data) {
	string line;
	vector<string> splittedLine;
	shared_ptr<Piece> piece;
	int piecevals[PIECE_NUM_FIELDS]; //array for saving piece values
	data.created = new bool[data.numOfPieces]; //todo smart allocate
	for (int i = 0; i < data.numOfPieces; data.created[i++] = false)
		;
	for (int i = 0; i < NUM_OF_CORNERS; data.edges[i++] = 0)
		;
	//-----------------------READ REST OF INPUT FILE-----------------------
	while (getline(inFile, line) && data.pieceCount < data.numOfPieces) {
		if (line.empty())
			continue;
		splittedLine = split(line);
		if (!isInt(splittedLine.at(0))) //check if format is legal
			data.nonDigitID.push_back(splittedLine.at(0));
		else {
			int id = stoi(splittedLine.at(0));
			if (id > data.numOfPieces || id <= 0)
				data.wrongID.push_back(id);
			else if (CheckLineFormat(piecevals, splittedLine)) {
				if (!puzzle.setPieceAtTree(piecevals, data.created, piece))
					data.duplicates.push_back(piecevals[0]); //ID already taken
				else {
					data.pieceCount++;
					data.oneRowSol &= markOneRowSolution(puzzle.isRotate(), piece);
					data.oneColSol &= markOneColSolution(puzzle.isRotate(), piece);
					puzzle.setOneRowColSolution(data.oneRowSol, data.oneColSol);
					MarkEdges(piece, data.edges);
					MarkCorners(piece, data.corners, puzzle.isRotate());
					for (int i = 1; i < PIECE_NUM_FIELDS; i++)
						data.sumOfEdges += piecevals[i];
				}
			} else {   //if format error add a garbage piece so it wont be missing
				data.created[id] = true;
				data.wrongFormat.push_back(make_pair(splittedLine.at(0), line));
				data.pieceCount++;
			}
		}
	}
	delete[] data.created; //donot forget
}

bool puzzleUtils::markOneRowSolution(bool rotate, shared_ptr<Piece>& p) {
	if (p->getTop() != 0 || p->getBottom() != 0) {
		if (rotate) {
			if ((p->getLeft() == 0 && p->getRight() == 0))
				return true;
		}
		return false;
	}
	return true;
}

bool puzzleUtils::markOneColSolution(bool rotate, shared_ptr<Piece>& p) {
	if (p->getLeft() != 0 || p->getRight() != 0) {
		if (rotate) {
			if ((p->getTop() == 0 && p->getBottom() == 0))
				return true;
		}
		return false;
	}
	return true;
}

bool puzzleUtils::preprocessPuzzle(Puzzle &puzzle, ifstream &inFile, ofstream &outFile) {
	puzzleParamaters data;
	data.numOfPieces = readNumOfPiecesFromFile(inFile, outFile); //read first line
	puzzle.setNumOfPieces(data.numOfPieces); //initialize a puzzle object
	readPiecesFromFile(puzzle, inFile, data); //read rest of file to puzzle
	inFile.close();
	return errorHandling(puzzle, outFile, data); //check and report for errors
}

bool puzzleUtils::errorHandling(Puzzle &puzzle, ofstream &outFile, puzzleParamaters &data) {
	bool cansolve = true; //flag if solution is possible
	cansolve &= checkMissingElements(puzzle, outFile, data.numOfPieces, data.pieceCount, data.created); //Missing elements
	cansolve &= checkWrongID(puzzle, outFile, data.wrongID); //Wrong element IDs
	cansolve &= checkWrongFormat(outFile, data.wrongFormat); //Wrong elements format
	cansolve &= checkNonDigitID(outFile, data.nonDigitID); //non digit elements format
	cansolve &= checkDuplicates(outFile, data.duplicates); //duplicate elements format
	if (!cansolve)
		return false; // critical format errors no need to proceed
	cansolve &= checkWrongStraightEdges(outFile, data.numOfPieces, data.edges, puzzle.isRotate()); //wrong number of straight edges
	cansolve &= checkMissingCorners(outFile, data.corners, data.numOfPieces, data.oneRowSol, data.oneColSol); //missing corner element: <TL><TR><BL><BR>
	cansolve &= checkSumOfEdges(outFile, data.sumOfEdges); //sum of edges is not zero
	return cansolve;
}

bool puzzleUtils::checkSingleSquare(vector<int> corners[]) {
	for (int i = 0; i < NUM_OF_CORNERS; i++)
		if (corners[i].size() != 1 || corners[i].at(0) != 0)
			return false;
	return true;
}

void puzzleUtils::MarkCorners(shared_ptr<Piece>& p, vector<int> corners[], bool rotate) {
	int numOfRotates = rotate ? NUM_OF_CORNERS : 1;
	for (int angle = 0; angle < numOfRotates * ROTATE_DEGREE; angle += ROTATE_DEGREE) {
		if (p->getTop(angle) == 0 && p->getLeft(angle) == 0) {     //check TL
			if (p->getRight(angle) == 0)					   //check TLTR
				corners[TLTR].push_back(p->getId());
			if (p->getBottom(angle) == 0) 				   //check TLBL
				corners[TLBL].push_back(p->getId());
			corners[TL].push_back(p->getId());
		}
		if (p->getTop(angle) == 0 && p->getRight(angle) == 0)      //check TR
			corners[TR].push_back(p->getId());
		if (p->getBottom(angle) == 0 && p->getLeft(angle) == 0) 	 //check BL
			corners[BL].push_back(p->getId());
		if (p->getBottom(angle) == 0 && p->getRight(angle) == 0) { //check BR
			if (p->getLeft(angle) == 0)					    //check BRBL
				corners[BLBR].push_back(p->getId());
			if (p->getTop(angle) == 0)						//check TRBR
				corners[TRBR].push_back(p->getId());
			corners[BR].push_back(p->getId());
		}
	}
}

bool puzzleUtils::checkMissingCorners(ofstream &out, vector<int> corners[], int numPieces, bool &oneRowSol, bool &oneColSol) {
	if (numPieces == 1 && checkSingleSquare(corners))
		return true;
	int result[2 * NUM_OF_CORNERS] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	CheckCorners(corners, result);
	bool isMissing = true;
	for (int i = 0; i < NUM_OF_CORNERS; i++)
		for (int j = i + 1; j < NUM_OF_CORNERS; j++)
			if (result[i] == result[j]) //if any two corners are equal
				result[j] = -1;
	//one col solution is possible
	if (result[TLTR] != result[BLBR] && result[TLTR] != -1 && result[BLBR] != -1) {
		oneColSol &= isMissing;
		return isMissing;
	}
	//one row solution is possible
	if (result[TLBL] != result[TRBR] && result[TLBL] != -1 && result[TRBR] != -1) {
		oneRowSol &= isMissing;
		return isMissing;
	}
	//check for any missin corners of anytime
	if (result[TL] == -1) {
		out << "Cannot solve puzzle: missing corner element: TL" << endl;
		isMissing = false;
	}
	if (result[TR] == -1) {
		out << "Cannot solve puzzle: missing corner element: TR" << endl;
		isMissing = false;
	}
	if (result[BL] == -1) {
		out << "Cannot solve puzzle: missing corner element: BL" << endl;
		isMissing = false;
	}
	if (result[BR] == -1) {
		out << "Cannot solve puzzle: missing corner element: BR" << endl;
		isMissing = false;
	}
	return isMissing;

}

void puzzleUtils::CheckCorners(vector<int> corners[], int res[]) {
	//todo rotate support
	size_t tlsize = corners[TL].size() > 0 ? corners[TL].size() : 1;
	size_t trsize = corners[TR].size() > 0 ? corners[TR].size() : 1;
	size_t blsize = corners[BL].size() > 0 ? corners[BL].size() : 1;
	size_t brsize = corners[BR].size() > 0 ? corners[BR].size() : 1;
	//check if theres 4 different corners to use (a square solution)
	for (size_t h = 0; h < tlsize; h++) {
		if (corners[TL].size() > 0)
			res[TL] = corners[TL].at(h);
		for (size_t i = 0; i < trsize; i++) {
			if (corners[TR].size() > 0 && corners[TR].at(i) != res[TL])
				res[TR] = corners[TR].at(i);
			for (size_t j = 0; j < blsize; j++) {
				if (corners[BL].size() > 0 && corners[BL].at(j) != res[TR] && corners[BL].at(j) != res[TL])
					res[BL] = corners[BL].at(j);
				for (size_t k = 0; k < brsize; k++) {
					if (corners[BR].size() > 0 && corners[BR].at(k) != res[TR] && corners[BR].at(k) != res[TL]
							&& corners[BR].at(k) != res[BL]) {
						res[BR] = corners[BR].at(k);
					}
				}
			}
		}
	}
	size_t tlersize = corners[TLTR].size() > 0 ? corners[TLTR].size() : 1;
	size_t blbrsize = corners[BLBR].size() > 0 ? corners[BLBR].size() : 1;
	//check if theres 2 different corners to use (a column solution)
	for (size_t n = 0; n < tlersize; n++) {
		if (corners[TLTR].size() > 0)
			res[TLTR] = corners[TLTR].at(n);
		for (size_t m = 0; m < blbrsize; m++) {
			if (corners[BLBR].size() > 0 && corners[BLBR].at(m) != res[TLTR])
				res[BLBR] = corners[BLBR].at(m);
		}
	}
	size_t trbrsize = corners[TRBR].size() > 0 ? corners[TRBR].size() : 1;
	size_t tlblsize = corners[TLBL].size() > 0 ? corners[TLBL].size() : 1;
	//check if theres 2 different corners to use (a row solution)
	for (size_t n = 0; n < trbrsize; n++) {
		if (corners[TRBR].size() > 0)
			res[TRBR] = corners[TRBR].at(n);
		for (size_t m = 0; m < tlblsize; m++) {
			if (corners[TLBL].size() > 0 && corners[TLBL].at(m) != res[TRBR])
				res[TLBL] = corners[TLBL].at(m);
		}
	}
}

void puzzleUtils::MarkEdges(shared_ptr<Piece>& p, int edges[]) {
	if (p->getLeft() == 0)
		edges[LEFT]++;
	if (p->getTop() == 0)
		edges[TOP]++;
	if (p->getRight() == 0)
		edges[RIGHT]++;
	if (p->getBottom() == 0)
		edges[BOTTOM]++;
}

bool puzzleUtils::checkWrongFormat(ofstream &outFile, vector<pair<string, string> > &wrongFormat) {
	if (wrongFormat.size() == 0)
		return true;
	for (unsigned int i = 0; i < wrongFormat.size(); i++)
		outFile << "Puzzle ID " << wrongFormat.at(i).first << " has wrong data: " << wrongFormat.at(i).second << endl;
	return false;
}

bool puzzleUtils::checkNonDigitID(ofstream &outFile, vector<string> &nonDigitID) {
	if (nonDigitID.size() == 0)
		return true;
	outFile << "Non digit IDs: ";
	for (unsigned int i = 0; i < nonDigitID.size(); i++) {
		outFile << nonDigitID.at(i);
		if (i != nonDigitID.size() - 1)
			outFile << ", ";
	}
	outFile << endl;
	return false;
}

bool puzzleUtils::checkDuplicates(ofstream &outFile, vector<int> &duplicates) {
	if (duplicates.size() == 0)
		return true;
	outFile << "Duplicate IDs: ";
	for (unsigned int i = 0; i < duplicates.size(); i++) {
		outFile << duplicates.at(i);
		if (i != duplicates.size() - 1)
			outFile << ", ";
	}
	outFile << endl;
	return false;
}

bool puzzleUtils::checkWrongID(const Puzzle &puzzle, ofstream &outFile, vector<int> &wrongID) {
	if (wrongID.size() == 0)
		return true;
	outFile << "Puzzle of size " << puzzle.getNumOfPieces() << " cannot have the following IDs: ";
	for (unsigned int i = 0; i < wrongID.size(); i++) {
		outFile << wrongID.at(i);
		if (i != wrongID.size() - 1)
			outFile << ", ";
	}
	outFile << endl;
	return false;
}

bool puzzleUtils::checkMissingElements(const Puzzle &puzzle, ofstream &outFile, int pieceCount, int numOfPieces, bool created[]) {
	if (pieceCount == numOfPieces)
		return true;
	int numOfMissing = numOfPieces - pieceCount;
	outFile << "Missing puzzle element(s) with the following IDs: ";
	for (int i = 0; i < puzzle.getNumOfPieces() && numOfMissing > 0; i++) {
		if (created[i] == false) {
			outFile << i + 1;
			if (numOfMissing != 1)
				outFile << ", ";
			numOfMissing--;
		}
	}
	outFile << endl;
	return false;
}

bool puzzleUtils::checkWrongStraightEdges(ofstream &outFile, int numOfPieces, int edges[], bool rotate) {
	int minedges = ((int) (sqrt(numOfPieces))) * 4;
	int sum;
	for (int i = 0; i < NUM_OF_CORNERS; sum += edges[i++]);
	if (sum > minedges) {
		if (!rotate) {
			if ((edges[RIGHT] - edges[LEFT] == 0) && (edges[TOP] - edges[BOTTOM] == 0)) {
				return true;
			} else {
				if (sum % 2 == 0)
					return true;
			}
		}
	}
	outFile << "Cannot solve puzzle: wrong number of straight edges" << endl;
	return false;
}

bool puzzleUtils::checkSumOfEdges(ofstream &outFile, int sumOfEdges) {
	if (sumOfEdges == 0)
		return true;
	outFile << "Cannot solve puzzle: sum of edges is not zero" << endl;
	return false;
}

bool puzzleUtils::ErrorCannotSolve(ofstream &outFile) {
	outFile << "Cannot solve puzzle: it seems that there is no proper solution" << endl;
	return false;
}
