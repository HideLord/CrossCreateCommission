#pragma once
#include<iostream>

using namespace std;

const int MaxSegTreeSize = 10000;

template <int N, typename T> class SegTree {

public:
	pair<double, int> Tr[3000];
	SegTree() {
		for (int i = N; i < N*2; i++) {
			Tr[i].first = -2000000000;
			Tr[i].second = i - N;
		}
	}
	inline void update(int ind, double val) {
		Tr[ind += N].first = val;
		ind >>= 1;
		while (ind) {
			Tr[ind] = max(Tr[ind << 1], Tr[ind << 1|1]);
			ind >>= 1;
		}
	}
	inline int get() const {
		return Tr[1].second;
	}
};