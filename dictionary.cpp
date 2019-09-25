#include "dictionary.h"
#include "Constants.h"

int Dictionary::levenstein(string a, string b) {
	vector<vector<int>> dp(a.length()+1, vector<int>(b.length()+1, 0));
	for (int i = 0; i <= a.length(); i++) {
		for (int j = 0; j <= b.length(); j++) {
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
		cout << "Failed to load dictionary: could not open file!" << endl;
		return;
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
		dirtyDict[clean] = w;
		explanationDict[clean] = expl;
		allWords.push_back(clean);
	}
	fin.close();
}

Dictionary::Dictionary()
{
}


Dictionary::~Dictionary()
{
}
