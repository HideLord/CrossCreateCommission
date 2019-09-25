#include "SolverService.h"
#include <functional>
#include <Chrono>

Crossword SolverService::Solve(Crossword unsolved)
{
	cross_ = unsolved;
	thread listener_thread([this]() { Listen(); }); // Listen for outside commands
	listener_thread.detach();
	thread printer_thread([this]() { PrintEvery(chrono::milliseconds(20)); }); // Print crossword every 20ms

	Prepare();
	StartSolving();

	if(printer_thread.joinable()) printer_thread.join();

	return cross_;
}

void SolverService::Prepare()
{
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
			if (neighbor[posIndex][i] != -1) {
				dictBackup[i] = cross_.areas[neighbor[posIndex][i]].dictIndex;
				int newDictIndex = dict_.GetDictIndex(cross_.areas[neighbor[posIndex][i]]);
				if (newDictIndex == -1) {
					for (; i >= 0; i--) {
						*letters[i] = backup[i];
						if (neighbor[posIndex][i] != -1) 
							cross_.areas[neighbor[posIndex][i]].dictIndex = dictBackup[i];
					}
					return false;
				}
			}
		}
	}
}

bool SolverService::IsReady() const
{
	int temp = NumPositions_ - 32, i = 0;
	for (; temp > 0; temp -= 32, i++) 
		if ((IsComplete_[i] & 0xFFFFFFFF) != 0xFFFFFFFF) // Not every bit in the integer is set
			return false;
	if ((1 << (temp + 32)) - 1 != IsComplete_[i]) return false;
	return true;
}

int SolverService::GetNextPosIndex() const
{
	return 0;
}
