#pragma once
#include<iostream>

using namespace std;

const int MaxSegTreeSize = 10000;

template <int N, typename T> class SegTree {
	T t[MaxSegTreeSize * 2];
public:
	void Modify(int p, T value) {  // set value at position p
		for (t[p += N] = value; p > 1; p >>= 1) t[p >> 1] = max(t[p], t[p ^ 1]);
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
		int res = l+N+1;
		for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
			if (l & 1) res = (t[res] > t[l++]?res:l-1);
			if (r & 1) res = (t[res] > t[--r]?res:r);
		}
		return res;
	}
};