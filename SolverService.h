#pragma once

#include "ISolver.h"
#include "crossword.h"
#include "dictionary.h"

#include <thread>
#include <vector>
#include <algorithm>

class SolverService : ISolver {
public:
	virtual Crossword Solve(Crossword unsolved) override;
private:
	int NumPositions_, currIndex_, currPen_, penThreshold_;
	unsigned int IsComplete_[15]; // bitset 15*32 = 480 word limit
	int wordIndex_[480];
	int positionIndices_[480];
	int penalty_[60000];
	bool usedIndices_[60000];
	Crossword cross_;
	Dictionary dict_;
private:
	void Prepare();
	void StartSolving();
	void PrintEvery(chrono::milliseconds ms) const;
	void Listen();
	bool TestPut(int posIndex, unsigned short wordIndex);
	bool IsReady() const;
	int GetNextPosIndex() const;
};