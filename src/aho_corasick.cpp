#include <bits/stdc++.h>

using namespace std;

const long long int alphabet_size = 256;

// go_to: long long int key
// state = floor(key/alphabet_size)
// char/edge/input = key%alphabet_size

unordered_map< long long int ,long long int> go_to, failure;
unordered_map<long long int, set<long long int> > output;
unordered_map<string, long long int> get_index;
unordered_map<long long int, string> get_string;
long long int new_state, index_pattern;


void clear() {
	go_to.clear();
	failure.clear();
	output.clear();
	get_index.clear();
}

long long int hash_pair(long long int state, long long int  c) {
	return ((state*alphabet_size)+c);
}

void enter(string pattern) {

	long long int state = 0, j= 0, m = pattern.size();

	while(j < m && go_to.find(hash_pair(state, pattern[j])) != go_to.end()) {
		state = go_to[hash_pair(state, pattern[j])];
		j++;
	}

	for(long long int i = j; i < m; i++) {
		new_state++;
		go_to.insert({hash_pair(state, pattern[i]),new_state});
		state = new_state;
	}

	if(output.find(state) == output.end()) output.insert({state,set<long long int>()});

	output[state].insert(get_index[pattern]);
}

void build_go_to(vector<string> patterns) {

	new_state = 0, index_pattern = 0;

	for(auto pattern : patterns) {
		get_index.insert({pattern,index_pattern});
		get_string.insert({index_pattern++, pattern});
		enter(pattern);
	}

	for(long long int i=0; i<alphabet_size; i++) {
		if(go_to.find(hash_pair(0, i)) == go_to.end()) {
			go_to.insert({hash_pair(0, i),0});
		}
	}
}

void build_failure() {

	queue<int> q;
	long long int i, r, s, state;

	for(i = 0; i<alphabet_size; i++) {
		s = go_to[hash_pair(0,i)];
		if(s != 0) {
			q.push(s);
			failure.insert({s,0});

		}
	}

	while(!q.empty()) {
		r = q.front();
		q.pop();
		for(i = 0; i<alphabet_size; i++) {
			if(go_to.find(hash_pair(r,i)) != go_to.end()) {
				long long int s = go_to[hash_pair(r,i)];
				q.push(s);
				state = failure[r];
				while(go_to.find(hash_pair(state,i)) == go_to.end() ) {
					state = failure[state];
				}
				failure[s] = go_to[hash_pair(state,i)];
				output[s].insert(output[failure[s]].begin(), output[failure[s]].end());
			}
		}
	}
}

void aho_corasick(string text, vector<string> patterns) {
	
	int n = text.size(), state = 0;
	build_go_to(patterns);
	build_failure();

	for(int i=0;i<n;i++) {
		while(go_to.find(hash_pair(state,text[i])) == go_to.end()) {
			state = failure[state];
		}
		state = go_to[hash_pair(state, text[i])];
		if(!output[state].empty()) {
			cout << "ocorrence at position " << i << " of patterns ";
			int i = 0;
			for(auto out : output[state]) {
				if(i++ > 0) cout << ", ";
				cout << get_string[out];
			} 
			cout << endl;
		}
	}
}

/*int main() {
	vector<string> patterns = {"he","she","his","hers"};
	string text = "ushershis";
	build_go_to(patterns);
	for(auto value : go_to) {
		if(value.second!=0) cout << "state: " << floor(value.first/alphabet_size) << ", char: " << (char)(value.first%alphabet_size) << ", next_state: " << value.second << endl;
	}
	cout << endl;
	build_failure();
	for(auto value : failure) {
		cout << "state: " << value.first << ", failure_state: " << value.second << endl;
	}
	cout << endl;
	
	for(auto value : output) {
		if(!value.second.empty()) {
			cout << "state: " << value.first << ", output: ";
			for(auto s : value.second) {
				cout << get_string[s] << " ";
			} 
			cout << endl;
		}
	}
	
	cout << "Text: "<< text << endl;
	cout << "Patterns: ";
	for(int i=0; i < patterns.size(); i++) {
		if(i>0) cout << ", ";
		cout << patterns[i];
	}
	cout << endl;
	aho_corasick(text, patterns);
	return 0;
}*/