#include <tuple>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

long ShiftOr(char* pattern, char* text, long** C);
long** buildMasks(char* pattern);
long WuManber(char* pattern, char* text, long** C, int r);
tuple< unordered_map< int ,int>, vector<int> , unordered_map< int ,int>, vector<int>, unordered_map<int, set<int> >> build_aho_corasick(vector<string> patterns);
vector<long> aho_corasick(string text, int line, vector<long> &ans, unordered_map< int ,int> &go_to, vector<int> &go_to_arr, unordered_map< int ,int> &failure, vector<int> &failure_arr, unordered_map<int, set<int> > &output);
bool aho_corasick(string text, int line, unordered_map< int ,int> &go_to, vector<int> &go_to_arr, unordered_map< int ,int> &failure, vector<int> &failure_arr, unordered_map<int, set<int> > &output);
tuple< unordered_map<int, int>, vector<int> , set<int> > build_ukkonen_fsm(string pattern, int r);
long ukkonen(string text, int line, unordered_map<int, int> &T, vector<int>  &T_arr, set<int> &F);