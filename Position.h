#pragma once
#include <vector>
using namespace std;

typedef unsigned char uc;

class Position {
public:
	vector<uc*> letters;
	bool hor;
	string toString() const{
		string res(letters.size(), 0);
		for (size_t i = 0; i < letters.size(); i++)
			res[i] = *letters[i];
		return res;
	}
	inline string operator=(string S) {
		for (size_t i = 0; i < letters.size(); i++)
			*letters[i] = S[i];
		return S;
	}
};