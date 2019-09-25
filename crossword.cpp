#include "crossword.h"

void Crossword::loadWords(){
	areas.clear();
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			int start = j;
			for (; j < M && !isBox(i, j); ++j);
			if (j - start <= 1) continue;
			Position newPos;
			newPos.hor = 1;
			for (; start < j; ++start)newPos.letters.push_back( &board[i][start] );
			areas.push_back(newPos);
		}
	}

	for (int j = 0; j < M; ++j) {
		for (int i = 0; i < N; ++i) {
			int start = i;
			for (; i < N && !isBox(i, j); ++i);
			if (i - start <= 1) continue;
			Position newPos;
			newPos.hor = 0;
			for (; start < i; ++start)newPos.letters.push_back( &board[start][j] );
			areas.push_back(newPos);
		}
	}
}

void Crossword::printASCII() const {
	if (name.empty())return;
	if (N == 0 || M == 0)return;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (isBox(i, j)) {
				cout << (j != 0 ? " " : "") << "|";
			}
			else {
				cout << (j != 0 ? " " : "") << board[i][j];
			}
		}
		cout << endl;
	}
}

void Crossword::load(string path){
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
	name = path.substr(0, path.size() - 4);
	fin.get(N);
	fin.get(M);
	for (int i = 0; i < N; i++) {
		board.push_back(vector<uc>());
		for (int j = 0; j < M; j++)board[i].push_back('0');
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			char c;
			fin.get(c);
			board[i][j] = uc(c);
			if (uc(board[i][j] + 64) >= cyrillicA)board[i][j] += 64;
			cout << (isBox(board[i][j]) ? uc('|') : board[i][j]) << " ";
		}
		cout << endl;
	}
	loadWords();
}

void Crossword::save(string path){
	if (path.empty()) {
		path = name + ".ctb";
	}
	ofstream fout(path,ios::binary);

	fout << N;
	fout << M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if(uc(board[i][j])>=cyrillicA)fout << uc(board[i][j] - 64);
			else fout << board[i][j];
		}
	}
	name = path;
	cout << "Saved successfully at " << name << "." << endl;
}

Crossword::Crossword()
{
}


Crossword::~Crossword()
{
}
