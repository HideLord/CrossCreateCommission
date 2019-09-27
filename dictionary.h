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
private:
	int process(string s);
public:
	vector<unsigned short>& GetMem(int dictIndex);
	int GetDictIndex(string s);
	static int levenstein(string a, string b);
public:
	vector<vector<unsigned short>> mem_;
	unordered_map<string, int> stringToDictIndex_;
	unordered_map<string, string> explanationDict_;
	unordered_map<string, string> dirtyDict_;
	vector<string> allWords_;
	void loadDict();
	Dictionary();
	~Dictionary();
};

