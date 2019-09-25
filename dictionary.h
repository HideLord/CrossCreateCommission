#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <string>
#include <string.h>
#include <unordered_map>
using namespace std;

class Dictionary
{
	using uc = unsigned char;
public:
	static int levenstein(string a, string b);
public:
	unordered_map<string, string> explanationDict;
	unordered_map<string, string> dirtyDict;
	vector<string> allWords;
	void loadDict();
	Dictionary();
	~Dictionary();
};

