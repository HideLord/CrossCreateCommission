#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <string>
#include <string.h>
#include <cassert>
#include "Constants.h"
#include "Position.h"
using namespace std;

using uc = unsigned char;

class Crossword
{
	void loadWords();
public:
	bool isBox(uc c) const {
		return c == boxChar || c == sboxChar;
	}
	bool isBox(int i, int j) const {
		return board[i][j] == boxChar || board[i][j] == sboxChar;
	}
	void printASCII() const;
	void load(string path);
	void save(string path = "");
	Crossword();
	~Crossword();

	string name;
	char N = 0, M = 0;

	vector<vector<uc>> board;
	vector<Position> areas;

	Crossword& operator=(const Crossword& cpy) {
		this->name = cpy.name;
		this->N = cpy.N;
		this->M = cpy.M;
		this->board = cpy.board;
		this->areas = cpy.areas;
		return *this;
	}
};

