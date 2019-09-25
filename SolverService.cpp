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

bool SolverService::IsReady() const
{
	int temp = NumPositions - 32, i = 0;
	for (; temp > 0; temp -= 32, i++) 
		if ((IsComplete[i] & 0xFFFFFFFF) != 0xFFFFFFFF) // Not every bit in the integer is set
			return false;
	if ((1 << (temp + 32)) - 1 != IsComplete[i]) return false;
	return true;
}
