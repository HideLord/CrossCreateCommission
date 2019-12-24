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
#include <unordered_set>
#include "robin_hood.h"
using namespace std;

class Dictionary
{
	using uc = unsigned char;
private:
	int process(string s);
	void AddSorted(unsigned short index);
public:
	vector<unsigned short>& GetMem(int dictIndex);
	int GetDictIndex(string s);
	static int levenstein(string a, string b);
public:
	int sizeCounter_ = 0;

	vector<unsigned short> sortedWordIndice_[256][256][50];
	vector<unsigned short> mem_[1000000];

	robin_hood::unordered_node_map<string, int> stringToDictIndex_;
	robin_hood::unordered_node_map<string, string> explanationDict_;
	robin_hood::unordered_node_map<string, string> dirtyDict_;

	vector<string> allWords_;

	void loadDict();
	Dictionary();
	~Dictionary();
};

