#include "SolverService.h"
#include <functional>
#include <iomanip>
#include <Chrono>
#include <wtypes.h>

Crossword SolverService::Solve(Crossword unsolved){
	cross_.Clone(unsolved);
	numAreas_ = static_cast<int>(cross_.areas_.size());

	dict_.loadDict();

	wordIndexForStep_.resize(numAreas_);
	wordUsedForStep_.resize(numAreas_);
	areaIndexForStep_.assign(numAreas_,-1);
	recoveryForStep_.resize(numAreas_);
	

	isComplete_.resize(numAreas_);
	used_.resize(dict_.allWords_.size());
	completeCounter_ = 0;

	// Check for completed words
	for (int i = 0; i < numAreas_; i++) {
		if (cross_.areas_[i].isComplete()) {
			completeCounter_++;
			isComplete_[i] = true;
		}
	}

	// Find the neighbouring areas
	FindNeighbours();

	// Give dicts and Update values
	for (int i = 0; i < numAreas_; i++) {
		if (!isComplete_[i])
			SetDictIndexAndSize(i);
		Update(i);
	}

	currentStep_ = 0;

	StartSolving();
	return cross_;
}

void SolverService::ReverseStep(int step){
	int areaIndex = areaIndexForStep_[step];

	// The step is before the current so it should be complete
	isComplete_[areaIndex] = false; // make it incomplete

	for (int i = 0; i < static_cast<int>(recoveryForStep_[step].size()); i++) {
		*cross_.areas_[areaIndex].letters_[i] = recoveryForStep_[step][i];
	}

	used_[wordUsedForStep_[step]] = false;
	wordIndexForStep_[step] = 0;
	areaIndexForStep_[step] = -1;

	SetDictIndexAndSize(areaIndex);

	for (int i = 0; i < static_cast<int>(areaNeighbours_[areaIndex].size()); i++) {
		if (isComplete_[areaNeighbours_[areaIndex][i]])continue;
		SetDictIndexAndSize(areaNeighbours_[areaIndex][i]);
	}

	Update(areaIndex);
	for (int i = 0; i < static_cast<int>(areaNeighbours_[areaIndex].size()); i++) {
		if (isComplete_[areaNeighbours_[areaIndex][i]])continue;
		Update(areaNeighbours_[areaIndex][i]);
	}
}

void SolverService::SkipBack(){
	int failedAreaIndex = areaIndexForStep_[currentStep_];

	while (!areNeighbours_[failedAreaIndex][areaIndexForStep_[currentStep_]] ) {
		ReverseStep(currentStep_);
		currentStep_--;
		if (currentStep_ < 0)break;
	}
}

bool SolverService::TestPut(int areaIndex, int wordIndex){
	string& word = dict_.allWords_[wordIndex];
	vector<pair<int,int>> affected;
	int i;
	bool notPossible = false;
	for (i = 0; i < static_cast<int>(word.size()); i++) 
		*cross_.areas_[areaIndex].letters_[i] = word[i];
	
	for (i = 0; i < static_cast<int>(areaNeighbours_[areaIndex].size()); i++) {
		affected.push_back({ cross_.areas_[areaNeighbours_[areaIndex][i]].dictIndex,
			cross_.areas_[areaNeighbours_[areaIndex][i]].dictSize });

		if (isComplete_[areaNeighbours_[areaIndex][i]])continue;

		if (!SetDictIndexAndSize(areaNeighbours_[areaIndex][i])) {
			notPossible = true;
			break;
		}
	}

	if (notPossible) {
		while ( i >= 0) {
			cross_.areas_[areaNeighbours_[areaIndex][i]].dictIndex = affected[i].first;
			cross_.areas_[areaNeighbours_[areaIndex][i]].dictSize = affected[i].second;
			i--;
		}
		return false;
	}

	isComplete_[areaIndex] = true;
	Update(areaIndex);

	for (i = 0; i < static_cast<int>(areaNeighbours_[areaIndex].size()); i++) {
		if (isComplete_[areaNeighbours_[areaIndex][i]])continue;
		Update(areaNeighbours_[areaIndex][i]);
	}
	return true;
}

void SolverService::FindNeighbours(){
	auto areNeighboursLambda = [this](int a, int b) {
		for (int i = 0; i < static_cast<int>(cross_.areas_[a].letters_.size()); i++) {
			for (int j = 0; j < static_cast<int>(cross_.areas_[b].letters_.size()); j++) {
				if (cross_.areas_[a].letters_[i] == cross_.areas_[b].letters_[j]) {
					// They have the same pointer in their letters which means they are neighbours
					return true;
				}
			}
		}
		return false;
	};

	areNeighbours_.assign(numAreas_,vector<char>(numAreas_));
	areaNeighbours_.assign(numAreas_, {});
	
	for (int i = 0; i < numAreas_; i++) {
		for (int j = 0; j < numAreas_; j++) {
			if (i == j)continue;
			if (areNeighboursLambda(i, j)) {
				areNeighbours_[i][j] = true;
				areaNeighbours_[i].push_back(j);
			}
		}
	}
}

bool SolverService::SetDictIndexAndSize(int areaIndex) {
	cross_.areas_[areaIndex].dictIndex = dict_.GetDictIndex(cross_.areas_[areaIndex].ToString());
	if (cross_.areas_[areaIndex].dictIndex==-1)return false;
	cross_.areas_[areaIndex].dictSize = dict_.mem_[cross_.areas_[areaIndex].dictIndex].size();
	return true;
}

void SolverService::Update(int areaIndex){
	Position& pos = cross_.areas_[areaIndex];
	double value = 0.0;
	if (isComplete_[areaIndex])value = -1;
	else {
		value = (double)pos.letters_.size()* (double)pos.letters_.size() / (double)pos.dictSize;
	}
	segTree_.update(areaIndex, value);
}

int SolverService::GetNextAreaIndex() {
	return segTree_.get();
}

void SolverService::StartSolving() {
	while (numAreas_ >= currentStep_+completeCounter_) {
		if (numIterations_++ % 100000 == 0) {
			system("cls");
			cross_.PrintASCII();
		}
		// The area index for the current step
		int& areaInd = areaIndexForStep_[currentStep_];
		// if the area index is -1 then we must initialize the step since this is before the first iteration of the loop
		bool initializing = false;
		if (areaInd == -1) {
			initializing = true;
			areaInd = GetNextAreaIndex();
			recoveryForStep_[currentStep_] = cross_.areas_[areaInd].ToString();
		}
		// The actual area for the current step
		Position& pos = cross_.areas_[areaInd];
		// The vector full of indice for the possible strings which match the pattern
		vector<int>& words = dict_.GetMem(pos.dictIndex);
		if (!initializing) { // This is not the first iteration of the loop
			used_[wordUsedForStep_[currentStep_]] = false;
			wordIndexForStep_[currentStep_]++;
		}
		// Find the next compatible string to put
		while (true) {
			if (wordIndexForStep_[currentStep_] >= static_cast<int>(words.size())) {
				SkipBack();
				if(currentStep_<0)currentStep_=0;
				break;
			}
			int wordInd = words[wordIndexForStep_[currentStep_]];
			if (used_[wordInd] || !TestPut(areaInd, wordInd)) {
				wordIndexForStep_[currentStep_] ++;
				continue;
			}

			wordUsedForStep_[currentStep_] = wordInd;
			used_[wordInd] = true;
			currentStep_++;

			
			break;
		}
	}
	system("cls");
	cross_.PrintASCII();
}

