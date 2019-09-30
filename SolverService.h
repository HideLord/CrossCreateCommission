#pragma once

//#define BITCOMPLETE

#include "ISolver.h"
#include "crossword.h"
#include "dictionary.h"

#include <thread>
#include <vector>
#include <algorithm>
#include "SegTree.h"

class SolverService : ISolver {
public:
	virtual Crossword Solve(Crossword unsolved) override;
private:
	int numPositions_, currIndex_, currPen_, penThreshold_, numCompleted_, numIterations_;
#ifdef BITCOMPLETE
	unsigned int isComplete_[15]; // bitset 15*32 = 480 word limit
#else
	bool isCompleteB_[480];
#endif // BITCOMPLETE
	int wordIndex_[480];
	int positionIndices_[480];
	int usedWordIndex_[480];
	int neighbors_[480][50]; // LengthLimit of a word is 50
	int penalty_[60000];
	bool usedIndices_[60000];
	bool areNeighbors_[480][480];
	string prevState_[480];
	SegTree<1024, double> segTree_;
	Crossword cross_;
	Dictionary dict_;
private:
	void Prepare();
	void StartSolving();
	void PrintEvery(chrono::milliseconds ms) const;
	void Listen();
	void UpdateValue(int posIndex);
	void SkipBack();
	void Reset();
	bool TestPut(int posIndex, unsigned short wordIndex);
	bool Restore(int posIndex, string & state);
	bool IsReady() const;
	int GetNextPosIndex() const;
};