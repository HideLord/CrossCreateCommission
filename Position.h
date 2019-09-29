#pragma once
#include <vector>
using namespace std;

typedef unsigned char uc;

class Position {
public:
	vector<uc*> letters_;
	bool isHor_;
	int dictIndex_ = -1, dictSize = 0;
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