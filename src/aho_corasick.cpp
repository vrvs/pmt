#include <iostream>
#include <cstring>
#include <tuple>
#include <map>
#include <set>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <vector>
#include "algorithms.h"

using namespace std;

const int alphabet_size = 256, arr_size = 200000;

// go_to: int key
// state = floor(key/alphabet_size)
// char/edge/input = key%alphabet_size

unordered_map<string, int> get_index;
unordered_map<int, string> get_string;


void insert_go_to(int key, int value, unordered_map< int ,int> &go_to, vector<int> &go_to_arr) {
	if(key < arr_size-10) go_to_arr[key] = value;
	else go_to.insert({key,value});
}

int get_go_to(int key, unordered_map< int ,int> &go_to, vector<int> &go_to_arr) {
	if(key < arr_size-10) return go_to_arr[key];
	else if(go_to.find(key) != go_to.end()) return go_to[key];
	else return -1;
}

void insert_failure(int key, int value, unordered_map< int ,int> &failure, vector<int> &failure_arr) {
	if(key < arr_size-10) failure_arr[key] = value;
	else failure.insert({key,value});
}

int get_failure(int key, unordered_map< int ,int> &failure, vector<int> &failure_arr) {
	if(key < arr_size-10) return failure_arr[key];
	else if(failure.find(key) != failure.end()) return failure[key];
	else return -1;
}

int hash_pair(int state, int  c) {
	if(c<0 || c >= alphabet_size) return 0;
	return ((state*alphabet_size)+c);
}

void enter(string pattern,unordered_map< int ,int> &go_to, vector<int> &go_to_arr,unordered_map<int, set<int> >  &output, int &new_state) {

	int state = 0, j= 0, m = pattern.size();

	while(j < m && get_go_to(hash_pair(state, pattern[j]), go_to, go_to_arr) != -1) {
		state = get_go_to(hash_pair(state, pattern[j]), go_to, go_to_arr);
		j++;
	}

	for(int i = j; i < m; i++) {
		new_state++;
		insert_go_to(hash_pair(state, pattern[i]),new_state, go_to, go_to_arr);
		state = new_state;
	}

	if(output.find(state) == output.end()) output.insert({state,set<int>()});

	output[state].insert(get_index[pattern]);
}

void build_go_to(vector<string> patterns, unordered_map< int ,int> &go_to, vector<int> &go_to_arr, unordered_map<int, set<int> >  &output) {
	int new_state = 0, index_pattern = 0;
	for(auto pattern : patterns) {
		get_index.insert({pattern,index_pattern});
		get_string.insert({index_pattern++, pattern});
		enter(pattern, go_to, go_to_arr, output, new_state);
	}

	for(int i=0; i<alphabet_size; i++) {
		if(get_go_to(hash_pair(0, i), go_to, go_to_arr) == -1) {
			insert_go_to(hash_pair(0, i),0, go_to, go_to_arr);
		}
	}
}

void build_failure(unordered_map< int ,int> &go_to, vector<int> &go_to_arr, unordered_map< int ,int> &failure, vector<int> &failure_arr, unordered_map<int, set<int> >  &output) {

	queue<int> q;
	int i, r, s, state;

	for(i = 0; i<alphabet_size; i++) {
		s = get_go_to(hash_pair(0,i), go_to, go_to_arr);
		if(s != 0) {
			q.push(s);
			insert_failure(s,0, failure, failure_arr);

		}
	}

	while(!q.empty()) {
		r = q.front();
		q.pop();
		for(i = 0; i<alphabet_size; i++) {
			if(get_go_to(hash_pair(r,i), go_to, go_to_arr) != -1) {
				int s = get_go_to(hash_pair(r,i), go_to, go_to_arr);
				q.push(s);
				state = get_failure(r, failure, failure_arr);
				while(get_go_to(hash_pair(state,i), go_to, go_to_arr) == -1 ) {
					state = get_failure(state, failure, failure_arr);
				}
				insert_failure(s,get_go_to(hash_pair(state,i), go_to, go_to_arr), failure, failure_arr);
				output[s].insert(output[get_failure(s, failure, failure_arr)].begin(), output[get_failure(s, failure, failure_arr)].end());
			}
		}
	}
}

tuple< unordered_map< int ,int>, vector<int> , unordered_map< int ,int>, vector<int>, unordered_map<int, set<int> >> build_aho_corasick(vector<string> patterns) {
	unordered_map< int ,int> go_to, failure;
	vector<int> go_to_arr(arr_size, -1), failure_arr(arr_size, -1);
	unordered_map<int, set<int> > output;
	build_go_to(patterns, go_to, go_to_arr, output);
	build_failure(go_to, go_to_arr, failure, failure_arr, output);
	return make_tuple(go_to, go_to_arr, failure, failure_arr, output);
}

vector<long> aho_corasick(string text, int line, vector<long> &ans, unordered_map< int ,int> &go_to, vector<int> &go_to_arr, unordered_map< int ,int> &failure, vector<int> &failure_arr, unordered_map<int, set<int> > &output) {
	
	int n = text.size(), state = 0;

	for(int i=0;i<n;i++) {
		while(get_go_to(hash_pair(state,text[i]), go_to, go_to_arr) == -1) {
			state = get_failure(state, failure, failure_arr);
		}
		state = get_go_to(hash_pair(state, text[i]), go_to, go_to_arr);
		if(!output[state].empty()) {
			//cout << "ocorrence at line " << line << " at position " << i << " of patterns ";
			//int i = 0;
			for(auto out : output[state]) {
				//if(i++ > 0) cout << ", ";
				//cout << get_string[out];
				ans[out]++;
			} 
			//cout << "\n";
		}
	}
	return ans;
}

bool aho_corasick(string text, int line, unordered_map< int ,int> &go_to, vector<int> &go_to_arr, unordered_map< int ,int> &failure, vector<int> &failure_arr, unordered_map<int, set<int> > &output) {
	
	int n = text.size(), state = 0;

	for(int i=0;i<n;i++) {
		while(get_go_to(hash_pair(state,text[i]), go_to, go_to_arr) == -1) {
			state = get_failure(state, failure, failure_arr);
		}
		state = get_go_to(hash_pair(state, text[i]), go_to, go_to_arr);
		if(!output[state].empty()) {
			return true;
		}
	}
	return false;
}

/*
int main() {
	ios::sync_with_stdio(0); cin.tie(0);
	vector<string> patterns = {"lovely"};
	string text = "lovely";
	cout << "Text: "<< text << "\n";
	cout << "Patterns: ";
	for(int i=0; i < patterns.size(); i++) {
		if(i>0) cout << ", ";
		cout << patterns[i];
	}
	cout << "\n";
	auto t = build_aho_corasick(patterns);
	ifstream infile;
	infile.open("shakespeare.txt");
	vector<int> ans(patterns.size(), 0);
	int line = 1;
	while(infile) {
		getline(infile, text);
		if(aho_corasick(text, line++,get<0>(t), get<1>(t), get<2>(t), get<3>(t), get<4>(t)) ) {
			cout << text << endl;
		}
	}
	cout << "\n" << "Occorrences:" << "\n";
	for(int i = 0; i < patterns.size(); i++) {
		cout << get_string[i] << " " << ans[i] << "\n";
	}
	infile.close();
	return 0;
}*/