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

typedef unsigned char uc;

class Crossword
{
	void LoadWords();
public:
	bool IsBox(uc c) const {
		return c == boxChar || c == sboxChar;
	}
	bool IsBox(int i, int j) const {
		return board_[i][j] == boxChar || board_[i][j] == sboxChar;
	}

	void PrintASCII() const;
	void Load(string path);
	void Save(string path = "");
	void Clear();
	void Clone(const Crossword& cpy);

	Crossword();
	~Crossword();

	string name_;
	char N = 0, M = 0;

	vector<vector<uc>> board_;
	vector<Position> areas_;

	[[deprecated("Use clone instead")]]
	Crossword& operator=(const Crossword& cpy) {
		this->name_ = cpy.name_;
		this->N = cpy.N;
		this->M = cpy.M;
		this->board_ = cpy.board_;
		this->areas_ = cpy.areas_;
		return *this;
	}
};

