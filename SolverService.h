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
	int NumPositions;
	unsigned int IsComplete[15]; // bitset 15*32 = 480 word limit
	Crossword cross_;
private:
	void Prepare();
	void StartSolving();
	void PrintEvery(chrono::milliseconds ms) const;
	void Listen();
	bool IsReady() const;
};