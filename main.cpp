#include <iostream>
#include "crossword.h"
#include "ISolver.h"
#include "SolverService.h"
#define NOMINMAX
#include <Windows.h>

using namespace std;

Crossword unsolved;
SolverService Solver;

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	unsolved.Load("Z:/Cross/xmn6877.ctb");
	unsolved.Clear();
	unsolved.PrintASCII();

	unsolved.Clone(Solver.Solve(unsolved));
}