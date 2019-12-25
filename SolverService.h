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
	Crossword cross_;
	Dictionary dict_;

	SegTree<1024, double> segTree_;

	int currPenalty;

	int numIterations_;
	int completeCounter_; // how many areas are complete
	int numAreas_; // number of areas
	int currentStep_;

	vector<int>   wordIndexForStep_; // in the 'recursion' this is the index of the current word
	vector<int>   wordUsedForStep_; // Which word was used for the specific step
	vector<int>   areaIndexForStep_; // tells you the index of the area for the step
	vector<string> recoveryForStep_; // tells you the string which was there before the start of the step

	vector<char> isComplete_; // tells you whether an area is complete
	vector<char> used_;  // tells you whether a word is used
	vector<vector<char>> areNeighbours_; // tells you whether two areas are neighbouring
	vector<vector<int>> areaNeighbours_; // All of the neighbours for a certain area

private:
	// Reverses the given step
	void ReverseStep(int step);
	// Skips back to the nearest area(step-wise) which is connected to the current area
	void SkipBack();
	// Temporarily put a word and test if anything breaks
	bool TestPut(int areaIndex, int wordIndex);
	// Find all the neighbours for each area
	void FindNeighbours();
	// Sets the dict index and size for an area. If there isn't a possible solution it returns false
	bool SetDictIndexAndSize(int areaIndex);
	// Updates the value of a certain area
	void Update(int areaIndex);
	// Starts the solving process
	void StartSolving();
	// Returns the next best area
	int GetNextAreaIndex();
};