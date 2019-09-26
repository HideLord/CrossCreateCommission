#include "SolverService.h"
#include <functional>
#include <Chrono>

Crossword SolverService::Solve(Crossword unsolved)
{
	cross_ = unsolved;
	//thread listener_thread([this]() { Listen(); }); // Listen for outside commands
	//listener_thread.detach();
	thread printer_thread([this]() { PrintEvery(chrono::milliseconds(20)); }); // Print crossword every 20ms

	Prepare();
	StartSolving();

	if(printer_thread.joinable()) printer_thread.join();

	return cross_;
}

void SolverService::Prepare()
{
	numPositions_ = cross_.areas.size();

	auto checkForSamePointer = [&](Position& A, Position& B) ->int const {
		if (A.hor == B.hor)return -1;
		for (int i = 0; i < A.letters.size(); i++)
			for (int j = 0; j < B.letters.size(); j++)
				if (A.letters[i] == B.letters[j])return i;
			
		return -1;
	};
	for (int i = 0; i < cross_.areas.size(); i++) {
		for (int j = 0; j < cross_.areas.size(); j++) {
			int res = checkForSamePointer(cross_.areas[i], cross_.areas[j]);
			if (res == -1)continue;
			neighbors_[i][res] = j;
		}
	}
	
	memset(usedIndices_, 0, sizeof usedIndices_);

	memset(wordIndex_, 0, sizeof wordIndex_);

	memset(isComplete_, 0, sizeof isComplete_);

	memset(positionIndices_, 0, sizeof positionIndices_);

	auto checkIfComplete = [&](Position& A) ->bool const {
		for (int i = 0; i < A.letters.size(); i++)if (*A.letters[i] == emptyChar)return false;
		return true;
	};
	for (int i = 0; i < cross_.areas.size(); i++) {
		if (checkIfComplete(cross_.areas[i])) {
			isComplete_[i / 32] |= (1 << (i % 32));
		}
	}
}

void SolverService::StartSolving() 
{
	while (!IsReady()) {
	beginOperation:;
		if (currIndex_ == -1)currIndex_ = 0, positionIndices_[0] = GetNextPosIndex();
		Position& currPos = cross_.areas[positionIndices_[currIndex_]];
		vector<unsigned short>& wordIndices = dict_.GetMem(currPos.dictIndex);
		for (int i = wordIndex_[currIndex_]; i < wordIndices.size(); i++) {
			if (usedIndices_[wordIndices[i]])						   continue;
			if (currPen_ + penalty_[wordIndices[i]] >= penThreshold_)  continue;
			if (!TestPut(positionIndices_[currIndex_], wordIndices[i]))continue;

			currIndex_ += 1;
			positionIndices_[currIndex_] = GetNextPosIndex();
			wordIndex_[currIndex_] = i;
			goto beginOperation;
		}
		wordIndex_[currIndex_] = 0;
		positionIndices_[currIndex_] = -1;
		currIndex_ -= 1;
	}
}

void SolverService::PrintEvery(chrono::milliseconds ms) const {
	if (IsReady())return;
	cross_.printASCII();
	this_thread::sleep_for(ms);
}

void SolverService::Listen()
{
	string command;
	while (1) {
		getline(cin, command);
		if (command == "e" || command == "exit") {

		}else	
		if (command == "p" || command == "pause") {

		}else
		if (command == "e" || command == "edit") {

		}
	}
}

bool SolverService::TestPut(int posIndex, unsigned short wordIndex)
{
	string & w = dict_.allWords[wordIndex];
	vector<unsigned char*> & letters = cross_.areas[posIndex].letters;
	vector<unsigned char> backup(letters.size());
	vector<int> dictBackup(letters.size(),-1);
 
	for (int i = 0; i < letters.size(); i++) {
		backup[i] = *letters[i];
		if (*letters[i] != w[i]) {
			*letters[i] = w[i];
			if (neighbors_[posIndex][i] != -1) {
				dictBackup[i] = cross_.areas[neighbors_[posIndex][i]].dictIndex;
				int newDictIndex = dict_.GetDictIndex(cross_.areas[neighbors_[posIndex][i]].ToString());
				if (newDictIndex == -1) {
					for (; i >= 0; i--) {
						*letters[i] = backup[i];
						if (neighbors_[posIndex][i] != -1) 
							cross_.areas[neighbors_[posIndex][i]].dictIndex = dictBackup[i];
					}
					return false;
				}
			}
		}
	}

	return true;
}

bool SolverService::IsReady() const
{
	int temp = numPositions_ - 32, i = 0;
	for (; temp > 0; temp -= 32, i++) 
		if ((isComplete_[i] & 0xFFFFFFFF) != 0xFFFFFFFF) // Not every bit in the integer is set
			return false;
	if ((1 << (temp + 32)) - 1 != isComplete_[i]) return false;
	return true;
}

int SolverService::GetNextPosIndex() const
{
	return 0;
}
