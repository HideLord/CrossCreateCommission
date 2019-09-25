#pragma once

class Crossword;

class ISolver {
public:
	virtual Crossword Solve(Crossword unsolved) = 0;
};