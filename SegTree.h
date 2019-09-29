#pragma once
#include<iostream>

using namespace std;

const int MaxSegTreeSize = 10000;

template <int N, typename T> class SegTree {
	T t[MaxSegTreeSize * 2];
	int maxInd[MaxSegTreeSize * 2];
public:
	SegTree() {
		for (int i = N; i < 2 * N; i++)maxInd[i] = i;
	}
	void ModifyVal(int p, T value) {  // set value at position p
		for (t[p += N] = value; p > 1; p >>= 1) t[p >> 1] = max(t[p], t[p ^ 1]);
	}
	void ModifyMax(int p, T value) {
		for (t[p += N] = value; p > 1; p >>= 1) {
			t[p >> 1] = max(t[p], t[p ^ 1]);
			maxInd[p >> 1] = (t[p] > t[p ^ 1] ? maxInd[p] : maxInd[p ^ 1]);
		}
	}
	T Query(int l, int r) const {  // max from interval [l, r)
		T res = T();
		for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
			if (l & 1) res = max(res,t[l++]);
			if (r & 1) res = max(res,t[--r]);
		}
		return res;
	}
	int QueryInd(int l, int r) const {
		T res = T();
		int ind = l+N;
		for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
			if (l & 1) {
				if (res < t[l])res = t[l], ind = maxInd[l];
				++l;
			}
			if (r & 1) {
				if (res < t[--r])res = t[r], ind = maxInd[r];
			}
		}
		return ind - N;
	}
};