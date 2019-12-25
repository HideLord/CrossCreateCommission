#pragma once
#include <vector>
#include "Constants.h"
using namespace std;

typedef unsigned char uc;

class Position {
public:
	vector<uc*> letters_;
	bool isHor_;
	int dictIndex; // The index of the dictionary which contains the possible strings which satisfy the pattern
	int dictSize;  // The size of the dictionary
	bool isComplete() {
		for (auto& l : letters_)
			if (*l == emptyChar)
				return false;
		return true;
	}
	string ToString() const{
		string res(letters_.size(), 0);
		for (size_t i = 0; i < letters_.size(); i++)
			res[i] = *letters_[i];
		return res;
	}
	inline string operator=(string S) {
		for (size_t i = 0; i < letters_.size(); i++)
			*letters_[i] = S[i];
		return S;
	}
};