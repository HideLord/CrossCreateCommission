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

	cin.tie(0);
	cout.tie(0);
	ios_base::sync_with_stdio(false);

	unsolved.Load("Z:/Cross/xbig227.ctb");
	unsolved.Clear();
	unsolved.PrintASCII();

	unsolved.Clone(Solver.Solve(unsolved));
}