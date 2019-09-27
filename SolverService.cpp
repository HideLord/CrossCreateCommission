#include "SolverService.h"
#include <functional>
#include <iomanip>
#include <Chrono>
#include <wtypes.h>

Crossword SolverService::Solve(Crossword unsolved)
{
	cross_.Clone(unsolved);

	try {
		cout << "Starting to prepare..." << endl;
		Prepare();
		cout << "Finished preparing without problems" << endl;
	}
	catch (char* exception) {
		cout << exception << endl;
		return unsolved;
	}

	thread listener_thread([this]() { Listen(); }); // Listen for outside commands
	listener_thread.detach();

	thread printer_thread([this]() { PrintEvery(chrono::milliseconds(20)); }); // Print crossword every 20ms

	cout << "Starting to solve..." << endl;
	StartSolving();
	if (printer_thread.joinable()) printer_thread.join();
	cout << "Solved" << endl;

	return cross_;
}

void SolverService::Prepare()
{
	try {
		dict_.loadDict();
	}
	catch (char* exception) {
		cout << exception << endl;
		throw exception;
	}
	numPositions_ = cross_.areas_.size();

	memset(neighbors_, -1, sizeof neighbors_);
	auto checkForSamePointer = [&](Position& A, Position& B) ->int const {
		if (A.isHor_ == B.isHor_)return -1;
		for (int i = 0; i < A.letters_.size(); i++)
			for (int j = 0; j < B.letters_.size(); j++)
				if (A.letters_[i] == B.letters_[j])return i;
			
		return -1;
	};
	for (int i = 0; i < cross_.areas_.size(); i++) {
		for (int j = 0; j < cross_.areas_.size(); j++) {
			int res = checkForSamePointer(cross_.areas_[i], cross_.areas_[j]);
			if (res == -1)continue;
			neighbors_[i][res] = j;
		}
	}

	numIterations_ = 0;

	currIndex_ = -1;

	currPen_ = 0;
	penThreshold_ = 150;

	std::memset(penalty_, 0, sizeof penalty_);

	std::memset(usedIndices_, 0, sizeof usedIndices_);

	std::memset(wordIndex_, 0, sizeof wordIndex_);

#ifdef BITCOMPLETE
	std::memset(isComplete_, 0, sizeof isComplete_);
#else
	numCompleted_ = 0;
	std::memset(isCompleteB_, 0, sizeof isCompleteB_);
#endif

	std::memset(positionIndices_, 0, sizeof positionIndices_);

	auto checkIfComplete = [&](Position& A) ->bool const {
		for (int i = 0; i < A.letters_.size(); i++)
			if (*A.letters_[i] == emptyChar)return false;
		return true;
	};
	for (int i = 0; i < cross_.areas_.size(); i++) {
		if (checkIfComplete(cross_.areas_[i])) {
#ifdef BITCOMPLETE
			isComplete_[i / 32] |= (1 << (i % 32));
#else // BITCOMPLETE
			numCompleted_ += 1;
			isCompleteB_[i] = true;
#endif
		}
		else {
			cross_.areas_[i].dictIndex_ = 
				dict_.GetDictIndex(cross_.areas_[i].ToString());
		}
		UpdateValue(i);
	}
}

void SolverService::StartSolving() 
{
	while (numCompleted_ != numPositions_) {
		++numIterations_;
		if (currIndex_ == -1)currIndex_ = 0, positionIndices_[0] = GetNextPosIndex();
		Position& currPos = cross_.areas_[positionIndices_[currIndex_]];
		vector<unsigned short>& wordIndices = dict_.GetMem(currPos.dictIndex_);
		if (wordIndex_[currIndex_] == 0) {
			prevState_[currIndex_] = currPos.ToString();
		}
		else {
			usedIndices_[wordIndices[wordIndex_[currIndex_]-(unsigned short)1]] = false;
		}

		for (int i = wordIndex_[currIndex_]; i < wordIndices.size(); i++) {
			if (usedIndices_[wordIndices[i]])						    continue;
			if (currPen_ + penalty_[wordIndices[i]] >= penThreshold_)   continue;
			if (!TestPut(positionIndices_[currIndex_], wordIndices[i])) continue;

			wordIndex_[currIndex_] = i+1;
			currIndex_ += 1;
			positionIndices_[currIndex_] = GetNextPosIndex();

			goto beginOperation;
		}

		Restore(positionIndices_[currIndex_], prevState_[currIndex_]);
		wordIndex_[currIndex_] = 0;
		currIndex_ -= 1;
	beginOperation:;
	}
}

void SolverService::PrintEvery(chrono::milliseconds ms) const {
	system("cls");
	do {
		COORD pos = { 0, 0 };
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(output, pos);

		cross_.PrintASCII();
		cout << "Num Iterations: " << numIterations_ << setw(10) << "\n";
		this_thread::sleep_for(ms);
	} while (!IsReady());
	COORD pos = { 0, 0 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);

	cross_.PrintASCII();
}

void SolverService::Listen()
{
	auto intersperser = chrono::milliseconds(10);
	string command;
	do {
		this_thread::sleep_for(intersperser);
		getline(cin, command);
		
		if (command == "e" || command == "exit") {

		}else	
		if (command == "p" || command == "pause") {

		}else
		if (command == "e" || command == "edit") {

		}
	} while (!IsReady());
}

void SolverService::UpdateValue(int posIndex)
{
	if (isCompleteB_[posIndex]) {
		segTree_.ModifyMax(posIndex, -1);
		return;
	}
	double val = 
		(double)100.0 / (dict_.GetMem(cross_.areas_[posIndex].dictIndex_).size()
			/ cross_.areas_[posIndex].letters_.size());
	segTree_.ModifyMax(posIndex, val);
}

bool SolverService::TestPut(int posIndex, unsigned short wordIndex)
{
	string & w = dict_.allWords_[wordIndex];
	vector<unsigned char*> & letters = cross_.areas_[posIndex].letters_;
	vector<unsigned char> backup(letters.size());
	vector<int> dictBackup(letters.size(),-2);
 
	for (int i = 0; i < backup.size(); i++) {
		backup[i] = *letters[i];
		if (*letters[i] != uc(w[i])) {
			*letters[i] = uc(w[i]);
			if (neighbors_[posIndex][i] != -1) {
				dictBackup[i] = cross_.areas_[neighbors_[posIndex][i]].dictIndex_;
				int newDictIndex = dict_.GetDictIndex(cross_.areas_[neighbors_[posIndex][i]].ToString());
				if (newDictIndex == -1) {
					for (; i >= 0; i--) {
						if (*letters[i] != backup[i]) {
							*letters[i] = backup[i];
							if (neighbors_[posIndex][i] != -1)
								cross_.areas_[neighbors_[posIndex][i]].dictIndex_ = dictBackup[i];
						}
					}
					return false;
				}
				else {
					cross_.areas_[neighbors_[posIndex][i]].dictIndex_ = newDictIndex;
				}
			}
		}
	}

	for (int i = 0; i < backup.size(); i++) {
		if (dictBackup[i] != -2) {
			UpdateValue(neighbors_[posIndex][i]);
		}
	}

#ifdef BITCOMPLETE
	isComplete_[posIndex / 32] |= (1 << (posIndex % 32));
#else
	numCompleted_ += 1;
	isCompleteB_[posIndex] = true;
#endif
	UpdateValue(posIndex);

	usedIndices_[wordIndex] = 1;

	return true;
}

bool SolverService::Restore(int posIndex, string& w)
{
#ifdef BITCOMPLETE
	isComplete_[posIndex / 32] &= (~(1 << (posIndex % 32)));
#else // BITCOMPLETE
	numCompleted_ -= 1;
	isCompleteB_[posIndex] = false;
#endif

	vector<unsigned char*>& letters = cross_.areas_[posIndex].letters_;

	for (int i = 0; i < letters.size(); i++) {
		if (*letters[i] != uc(w[i])) {
			*letters[i] = uc(w[i]);
			if (neighbors_[posIndex][i] != -1) {
				cross_.areas_[neighbors_[posIndex][i]].dictIndex_ = dict_.GetDictIndex(cross_.areas_[neighbors_[posIndex][i]].ToString());
				UpdateValue(neighbors_[posIndex][i]);
			}
		}
	}

	UpdateValue(posIndex);

	return true;
}

bool SolverService::IsReady() const
{
#ifdef BITCOMPLETE
	int temp = numPositions_ - 32, i = 0;
	for (; temp > 0; temp -= 32, i++) 
		if ((isComplete_[i] & 0xFFFFFFFF) != 0xFFFFFFFF) // Not every bit in the integer is set
			return false;
	if ((1 << (temp + 32)) - 1 != isComplete_[i]) return false;
	return true;
#else
	return numCompleted_ == numPositions_;
#endif
}

int SolverService::GetNextPosIndex() const
{
	return segTree_.QueryInd(0, numPositions_);
}
