#include <iostream>
#include <cstring>
#include <tuple>
#include <map>
#include <set>
#include <queue>
#include <unordered_map>
#include <vector>
#include "algorithms.h"

using namespace std;

const int alphabet_size = 256, arr_size = 200000;


int hash_pair_u(int index, int  c) {
	if(c<0 || c >= alphabet_size) return 0;
	return ((index*alphabet_size)+c);
}

void insert_T(int key, int value, unordered_map<int, int> &T, vector<int> &T_arr) {
	if(key < arr_size-10) T_arr[key] = value;
	else T.insert({key,value});
}

int get_T(int key, unordered_map<int, int> &T, vector<int> &T_arr) {
	if(key < arr_size-10) return T_arr[key];
	else if(T.find(key) != T.end()) return T[key];
	else return -1;
}

int phi(char a, char b) {
	if(a==b) return 0;
	return 1;
}

vector<int> next_column(vector<int> S, char a, string pattern, int r) {
	int m = pattern.size();
	vector<int>  S_l (m+1, 0);
	for(int i = 1; i <= m; i++) {
		S_l[i] = min(	min (	S[i-1] + phi(a, pattern[i-1]),
								S[i] + 1) ,
						min (	S_l[i-1] + 1,
								r + 1)
					);
	}
	return S_l;
}

tuple< unordered_map<int, int>, vector<int> , set<int> > build_ukkonen_fsm(string pattern, int r) {
	map<vector<int>, int> Q;
	set<int> F;
	unordered_map<int, int> T;
	vector<int> T_arr(arr_size, -1);
	int m = pattern.size(), i;
	vector<int> S (m+1, 0), S_l;
	for(i=0;i<=m;i++) {
		S[i] = i;
	}
	i = 0;
	Q.insert({S,i});
	queue<vector<int> > N;
	N.push(S);
	if(m<=r) {
		F.insert(i);
	}
	while(!N.empty()) {
		S = N.front(); N.pop();
		for(int a = 0; a < alphabet_size; a++) {
			S_l = next_column(S, a, pattern, r);
			if(Q.find(S_l) == Q.end()) {
				i++;
				Q.insert({S_l,i});
				N.push(S_l);
				if(S_l[m] <= r) {
					F.insert(i);
				}
			}
			insert_T(hash_pair_u(Q[S], a), Q[S_l], T, T_arr);
		}
	}
	return make_tuple( T, T_arr, F);
}

long ukkonen(string text, int line, unordered_map<int, int> &T, vector<int>  &T_arr, set<int> &F) {
	int n = text.size(), s = 0;
	long occ = 0;
	if(F.count(s)) {
		//cout << "ocorrence at line " << line << " at position " << s << '\n';
		occ++;
	}
	for(int i = 0; i < n; i++) {
		s = get_T(hash_pair_u(s,text[i]), T, T_arr);
		if(F.count(s)) {
			//cout << "ocorrence at line " << line << " at position " << i << '\n';
			occ++;
		}
	}
	return occ;
}

/*
int main() {
	ios::sync_with_stdio(0); cin.tie(0);
	string pattern = "adsasdasdasdlove", text;
	auto  t = build_ukkonen_fsm(pattern, 8);
	ifstream infile;
	infile.open("shakespeare.txt");
	int ans = 0;
	int line = 1;
	while(infile) {
		getline(infile, text);
		ans += ukkonen(text, line++, get<0>(t), get<1>(t), get<2>(t));
	}
	cout << "\n" << "Occorrences: " << ans << "\n";
	infile.close();
	return 0;
}*/