#include "puzzleUtil.h"

bool puzzleUtil::isInt(string &s) {
	for (size_t n = 0; n < s.length(); n++) {
		if (n == 0 && s[n] == '-') //number could be negertive
			continue;
		if (!isdigit(s[n]))
			return false;
	}
	return true;
}

string puzzleUtil::removespaces(const string &s) {
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

vector<string> puzzleUtil::split(const string &s, char delim) {
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

bool puzzleUtil::CheckLineFormat(int pvals[], vector<string> &splt) {
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

bool puzzleUtil::preprocessPuzzle(Puzzle &puzzle, ifstream &inFile, ofstream &outFile) {
	//-----------------FILE READING AND DATA PREPROCESSING-----------------
	bool cansolve = true;
	string line;
	int sumOfEdges = 0, numOfPieces = 0, pieceCount = 0, horizontal_edges = 0, vertical_edges = 0;
	vector<int> wrongID, duplicates;
	vector<int> corners[2 * NUM_OF_CORNERS];
	vector<string> splittedLine, nonDigitID;
	vector<pair<string, string> > wrongFormat;
	//get first line for number of elements
	while (line.empty())
		getline(inFile, line);
	splittedLine = split(removespaces(line), '='); //removes spacs and equal sign
	if (splittedLine.size() < 2 || splittedLine.at(0).compare("NumElements") != 0 || !isInt(splittedLine.at(1))) {
		outFile << "ERROR: invalid input file format" << endl;
		return false;
	}
	numOfPieces = stoi(splittedLine.at(1));
	int piecevals[PIECE_NUM_FIELDS]; //array for saving piece values
	puzzle.initPuzzle(numOfPieces); //initialize a puzzle object
	//-----------------------READ REST OF INPUT FILE-----------------------
	while (getline(inFile, line) && pieceCount < numOfPieces) {
		if (line.empty())
			continue;
		splittedLine = split(line);
		if (!isInt(splittedLine.at(0))) //check if format is legal
			nonDigitID.push_back(splittedLine.at(0));
		else {
			int id = stoi(splittedLine.at(0));
			if (id > numOfPieces || id <= 0)
				wrongID.push_back(id);
			else if (CheckLineFormat(piecevals, splittedLine)) {
				if (!puzzle.setPieceAtIndex(piecevals))
					duplicates.push_back(piecevals[0]); //ID already taken
				else {
					pieceCount++;
					MarkEdges(puzzle.getPieceAtIndex(piecevals[ID] - 1), horizontal_edges, vertical_edges);
					MarkCorners(puzzle.getPieceAtIndex(piecevals[ID] - 1), corners);
					for (int i = 1; i < PIECE_NUM_FIELDS; i++)
						sumOfEdges += piecevals[i];
				}
			}
			else
			{   //if format error add a garbage piece so it wont be missing
				piecevals[0]=id;
				puzzle.setPieceAtIndex(piecevals);
				wrongFormat.push_back(make_pair(splittedLine.at(0), line));
				pieceCount++;
			}

		}

	}
	inFile.close();
	//---------------------------ERROR HANDELING---------------------------
	cansolve = true; //flag if solution is possible
	if (pieceCount < numOfPieces) //Missing elements
		cansolve = ErrorMissingElements(puzzle, outFile, numOfPieces - pieceCount);
	if (wrongID.size() > 0) //Wrong element IDs
		cansolve = ErrorWrongID(puzzle, outFile, wrongID);
	if (wrongFormat.size() > 0) //Wrong elements format
		cansolve = ErrorWrongFormat(outFile, wrongFormat);
	if (nonDigitID.size() > 0) //non digit elements format
		cansolve = ErrorNonDigitID(outFile, nonDigitID);
	if (duplicates.size() > 0) //duplicate elements format
		cansolve = ErrorDuplicates(outFile, duplicates);
	if (!cansolve)
		return false;
	if (horizontal_edges != 0 || vertical_edges != 0) //wrong number of straight edges
		cansolve = ErrorWrongStraightEdges(outFile);
	//missing corner element: <TL><TR><BL><BR> ignoring special case of a single square
	if (numOfPieces != 1 || !checkSquare(puzzle.getPieceAtIndex(0))) {
		CheckCorners(corners, puzzle.getMissingCorners());
		cansolve = cansolve && ErrorMissingCorners(outFile, puzzle.getMissingCorners());
	}
	if (sumOfEdges != 0) //sum of edges is not zero
		cansolve = ErrorSumOfEdges(outFile);
	return cansolve;
}

bool puzzleUtil::checkSquare(const Piece* p) {
	return (p->getTop() == 0 && p->getLeft() == 0 && p->getBottom() == 0 && p->getRight() == 0);
}

void puzzleUtil::MarkCorners(const Piece* p, vector<int> corners[]) {
	if (p->getTop() == 0 && p->getLeft() == 0) {     //check TL
		if (p->getRight() == 0)					   //check TLTR
			corners[TLTR].push_back(p->getId());
		if (p->getBottom() == 0) 				   //check TLBL
			corners[TLBL].push_back(p->getId());
		corners[TL].push_back(p->getId());
	}
	if (p->getTop() == 0 && p->getRight() == 0)      //check TR
		corners[TR].push_back(p->getId());
	if (p->getBottom() == 0 && p->getLeft() == 0) 	 //check BL
		corners[BL].push_back(p->getId());
	if (p->getBottom() == 0 && p->getRight() == 0) { //check BR
		if (p->getLeft() == 0)					    //check BRBL
			corners[BLBR].push_back(p->getId());
		if (p->getTop() == 0)						//check TRBR
			corners[TRBR].push_back(p->getId());
		corners[BR].push_back(p->getId());
	}
}

bool puzzleUtil::ErrorMissingCorners(ofstream &outFile, int* res) {
	bool isMissing = true;
	for (int i = 0; i < NUM_OF_CORNERS; i++)
		for (int j = i + 1; j < NUM_OF_CORNERS; j++)
			if (res[i] == res[j]) //if any two corners are equal
				res[j] = -1;
	//one col solution is possible
	if (res[TLTR] != res[BLBR] && res[TLTR] != -1 && res[BLBR] != -1)
		return isMissing;
	//one raw solution is possible
	if (res[TLBL] != res[TRBR] && res[TLBL] != -1 && res[TRBR] != -1)
		return isMissing;
	//check for any missin corners of anytime
	if (res[TL] == -1) {
		outFile << "Cannot solve puzzle: missing corner element: TL" << endl;
		isMissing = false;
	}
	if (res[TR] == -1) {
		outFile << "Cannot solve puzzle: missing corner element: TR" << endl;
		isMissing = false;
	}
	if (res[BL] == -1) {
		outFile << "Cannot solve puzzle: missing corner element: BL" << endl;
		isMissing = false;
	}
	if (res[BR] == -1) {
		outFile << "Cannot solve puzzle: missing corner element: BR" << endl;
		isMissing = false;
	}
	return isMissing;

}

void puzzleUtil::CheckCorners(vector<int> corners[], int res[]) {
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

void puzzleUtil::MarkEdges(const Piece* p, int &horizontal_edges, int &vertical_edges) {
	if (p->getRight() == 0)
		++horizontal_edges;
	if (p->getLeft() == 0)
		--horizontal_edges;
	if (p->getTop() == 0)
		++vertical_edges;
	if (p->getBottom() == 0)
		--vertical_edges;
}

bool puzzleUtil::ErrorWrongStraightEdges(ofstream &outFile) {
	outFile << "Cannot solve puzzle: wrong number of straight edges" << endl;
	return false;
}

bool puzzleUtil::ErrorWrongFormat(ofstream &outFile, vector<pair<string, string> > &wrongFormat) {
	for (unsigned int i = 0; i < wrongFormat.size(); i++)
		outFile << "Puzzle ID " << wrongFormat.at(i).first << " has wrong data: " << wrongFormat.at(i).second << endl;
	return false;
}

bool puzzleUtil::ErrorNonDigitID(ofstream &outFile, vector<string> &nonDigitID) {
	outFile << "Non digit IDs: ";
	for (unsigned int i = 0; i < nonDigitID.size(); i++) {
		outFile << nonDigitID.at(i);
		if (i != nonDigitID.size() - 1)
			outFile << ", ";
	}
	outFile << endl;
	return false;
}

bool puzzleUtil::ErrorDuplicates(ofstream &outFile, vector<int> &duplicates) {
	outFile << "Duplicate IDs: ";
	for (unsigned int i = 0; i < duplicates.size(); i++) {
		outFile << duplicates.at(i);
		if (i != duplicates.size() - 1)
			outFile << ", ";
	}
	outFile << endl;
	return false;
}

bool puzzleUtil::ErrorWrongID(const Puzzle &puzzle, ofstream &outFile, vector<int> &wrongID) {
	outFile << "Puzzle of size " << puzzle.getNumOfPieces() << " cannot have the following IDs: ";
	for (unsigned int i = 0; i < wrongID.size(); i++) {
		outFile << wrongID.at(i);
		if (i != wrongID.size() - 1)
			outFile << ", ";
	}
	outFile << endl;
	return false;
}

bool puzzleUtil::ErrorMissingElements(const Puzzle &puzzle, ofstream &outFile, int numOfMissing) {
	outFile << "Missing puzzle element(s) with the following IDs: ";
	for (int i = 0; i < puzzle.getNumOfPieces() && numOfMissing > 0; i++) {
		if (puzzle.getPieceAtIndex(i) == nullptr) {
			outFile << i + 1;
			if (numOfMissing != 1)
				outFile << ", ";
			numOfMissing--;
		}
	}
	outFile << endl;
	return false;
}

bool puzzleUtil::ErrorSumOfEdges(ofstream &outFile) {
	outFile << "Cannot solve puzzle: sum of edges is not zero" << endl;
	return false;
}

bool puzzleUtil::ErrorCannotSolve(ofstream &outFile) {
	outFile << "Cannot solve puzzle: it seems that there is no proper solution" << endl;
	return false;
}
