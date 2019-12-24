#include "dictionary.h"
#include "Constants.h"
#include <cassert>

int Dictionary::process(string s){
	size_t sz = s.size();
	vector<unsigned short>& appropriate =
		sortedWordIndice_[uc(s[0])][uc(s[1])][sz];
	for (size_t k = 0; k < appropriate.size(); k++) {
		unsigned short i = appropriate[k];
		bool add = true;
		for (int j = 0; j < sz; j++) {
			if (s[j] == emptyChar)continue;
			if (s[j] != allWords_[i][j]) {
				add = false;
				break;
			}
		}
		if (add) mem_[sizeCounter_].push_back(i);
	}
	if (mem_[sizeCounter_].empty()) 
		return -1;
	return sizeCounter_++;
}

void Dictionary::AddSorted(unsigned short index){
	if (allWords_[index].size() < 2)return;
	size_t sz = allWords_[index].size();
	sortedWordIndice_[uc(allWords_[index][0])][uc(allWords_[index][1])][sz].push_back(index);
	sortedWordIndice_[uc(emptyChar)][uc(emptyChar)][sz].push_back(index);
	sortedWordIndice_[uc(allWords_[index][0])][uc(emptyChar)][sz].push_back(index);
	sortedWordIndice_[uc(emptyChar)][uc(allWords_[index][1])][sz].push_back(index);
}

vector<unsigned short>& Dictionary::GetMem(int dictIndex){
	return mem_[dictIndex];
}

int Dictionary::GetDictIndex(string s)
{
	if (stringToDictIndex_.count(s)) {
		return stringToDictIndex_[s];
	}
	else {
		return stringToDictIndex_[s] = process(s);
	}
}

int Dictionary::levenstein(string a, string b) {
	vector<vector<int>> dp(a.length()+1, vector<int>(b.length()+1, 0));
	for (size_t i = 0; i <= a.length(); i++) {
		for (size_t j = 0; j <= b.length(); j++) {
			if (min(i,j) == 0)dp[i][j] = 0;
			else {
				dp[i][j] = min(dp[i - 1][j] + 1, dp[i][j - 1] + 1);
				dp[i][j] = min(dp[i][j], dp[i - 1][j - 1] + (a[i-1] != b[j-1]));
			}
		}
	}
	return dp[a.length()][b.length()];
}

void Dictionary::loadDict() {

	ifstream fin;
	fin.open("Z:\\Cross\\BIGDICT.TXT");
	if(!fin.good()) fin.open("BIGDICT.txt");
	if (!fin.good()) {
		throw "Failed to load dictionary: could not open file!";
	}
	string w, expl;
	while (getline(fin, w, '\t')) {
		getline(fin, expl);
		string clean;
		for (size_t i = 0; i < w.size(); i++) {
			uc c = w[i];
			if (c >= cyrillicA - 96 && c < cyrillicA - 64)c += 32;
			if (c >= cyrillicA - 64 && c < cyrillicA - 32)c += 64, clean.push_back(c);
			if (uc(w[i]) >= cyrillicA - 96 && uc(w[i]) < cyrillicA - 32)w[i] += 64;
		}
		for (size_t i = 0; i < expl.size(); i++) {
			uc c = expl[i];
			if (c >= cyrillicA - 96 && c < cyrillicA - 32)c += 64;
			expl[i] = c;
		}
		dirtyDict_[clean] = w;
		explanationDict_[clean] = expl;
		allWords_.push_back(clean);
	}
	fin.close();
	random_shuffle(allWords_.begin(), allWords_.end());
	for (int i = 0; i < allWords_.size(); i++)AddSorted(i);
}

Dictionary::Dictionary()
{
}


Dictionary::~Dictionary()
{
}
