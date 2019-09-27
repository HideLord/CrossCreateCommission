#include "crossword.h"

void Crossword::LoadWords(){
	areas_.clear();
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			int start = j;
			for (; j < M && !IsBox(i, j); ++j);
			if (j - start <= 1) continue;
			Position newPos;
			newPos.isHor_ = 1;
			for (; start < j; ++start)newPos.letters_.push_back( &board_[i][start] );
			areas_.push_back(newPos);
		}
	}

	for (int j = 0; j < M; ++j) {
		for (int i = 0; i < N; ++i) {
			int start = i;
			for (; i < N && !IsBox(i, j); ++i);
			if (i - start <= 1) continue;
			Position newPos;
			newPos.isHor_ = 0;
			for (; start < i; ++start)newPos.letters_.push_back( &board_[start][j] );
			areas_.push_back(newPos);
		}
	}
}

void Crossword::PrintASCII() const {
	if (name_.empty())return;
	if (N == 0 || M == 0)return;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (IsBox(i, j)) {
				cout << (j != 0 ? " " : "") << "|";
			}
			else {
				cout << (j != 0 ? " " : "") << board_[i][j];
			}
		}
		cout << endl;
	}
}

void Crossword::Load(string path){
	while (path.size() < 5) {
		cout << "Invalid path. Try again:" << endl;
		cin >> path;
	}
	for(size_t i = 0; i < path.size(); i++)path[i] = tolower(path[i]);
	if (path.substr(path.size() - 4, 4) != ".ctb")
		path += ".ctb";
	ifstream fin(path, ios::binary);
	while (!fin.good()) {
		cout << "Invalid path " << path << ". Try again:" << endl;
		cin >> path;
		if (path.substr(path.size() - 4, 4) != ".ctb")
			path += ".ctb";
		fin.open(path, ios::binary);
	}
	name_ = path.substr(0, path.size() - 4);
	fin.get(N);
	fin.get(M);
	for (int i = 0; i < N; i++) {
		board_.push_back(vector<uc>());
		for (int j = 0; j < M; j++)board_[i].push_back('0');
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			char c;
			fin.get(c);
			board_[i][j] = uc(c);
			if (uc(board_[i][j] + 64) >= cyrillicA)board_[i][j] += 64;
			cout << (IsBox(board_[i][j]) ? uc('|') : board_[i][j]) << " ";
		}
		cout << endl;
	}
	LoadWords();
}

void Crossword::Save(string path){
	if (path.empty()) {
		path = name_ + ".ctb";
	}
	ofstream fout(path,ios::binary);

	fout << N;
	fout << M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if(uc(board_[i][j])>=cyrillicA)fout << uc(board_[i][j] - 64);
			else fout << board_[i][j];
		}
	}
	name_ = path;
	cout << "Saved successfully at " << name_ << "." << endl;
}

Crossword::Crossword()
{
}


Crossword::~Crossword()
{
}
