#include <iostream>
#include <fstream>
#include <getopt.h>
#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <cstring>
#include "algorithms.h"

using namespace std;

enum Algorithm { Shift_Or, Wu_Manber, Aho_Corasick, Ukkonen, Undefined };

string algorithm_name = "";
Algorithm algorithm = Undefined;
string patternfile = "";
bool count_occ = false;
bool use_pattern_file = false;
int edit_num = 0;
string pattern = "";
vector<string> textfiles = vector<string>(0);
vector<string> patterns = vector<string>(0);
int args_index = 1;

void PrintHelp(){
	cout << "Usage: ./pmt [options] pattern textfile [textfile...]" << endl << endl;
	cout << "Options:" << endl;
	cout << "  -e, --edit e-max                  An integer e-max corresponding to the edit distance" << endl;
	cout << "  -p, --pattern patternfile         Search for every pattern contained in patternfile" << endl;
	cout << "  -a, --algorithm algorithm-name    Uses the algorithm indicated by algorithm-name. Supported algorithms:" << endl;
	cout << "     - ShiftOr" << endl;
	cout << "     - AhoCorasick" << endl;
	cout << "     - WuManber" << endl;
	cout << "     - Ukkonen" << endl;
	cout << "  -c, --count                       Weather set, prints just the total occurrences of the pattern(s)" << endl;
}

void ProcessOptions(int argc, char **argv){

	string cur_patt;
	ifstream p_file;
	
	const char* short_opts = "e:p:a:ch";
	
	const option long_opts[] = {
		{"edit", required_argument, nullptr, 'e'},
		{"pattern", required_argument, nullptr, 'p'},
		{"algorithm", required_argument, nullptr, 'a'},
		{"count", no_argument, nullptr, 'c'},
		{"help", no_argument, nullptr, 'h'},
		{nullptr, no_argument, nullptr, 0}
	};
	
	while(true){
		const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
		
		if(opt == -1) break;
		else if(opt == 'h' || opt == 'c') args_index++;
		else args_index+=2;
		
		switch(opt){
			case 'e':
				try {
					edit_num = stoi(optarg);
				} catch (const exception &err) {
					throw "pmt: The edit number must be an integer.";
				}

				if(edit_num < 0){
					throw "pmt: The edit number can't be less than zero.";
				}

				break;

			case 'p':
				patternfile = optarg;
				p_file.open(patternfile);
				if(!p_file.good())
					throw "pmt: Pattern file doesn't exist";
					
				use_pattern_file = true;
				while(getline(p_file,cur_patt))
					patterns.push_back(cur_patt);
				p_file.close();

				break;

			case 'a':
				algorithm_name = optarg;
				if (!algorithm_name.compare("ShiftOr")){
					algorithm = Shift_Or;
				} else if (!algorithm_name.compare("WuManber")){
					algorithm = Wu_Manber;
				} else if (!algorithm_name.compare("AhoCorasick")) {
					algorithm = Aho_Corasick;
				} else if (!algorithm_name.compare("Ukkonen")) {
					algorithm = Ukkonen;
				} else {
					throw "pmt: Algorithm choice doesn't exist";
				}
				break;
				
			case 'c':
				count_occ = true;
				break;

			case 'h':
				PrintHelp();
				exit(0);
				break;

			default:
				break;
		}
	}
}

void ProcessParameters(int argc, char** argv){
	if((!use_pattern_file && args_index+1 < argc) || (use_pattern_file && args_index < argc)){
		if(!use_pattern_file){
			pattern = argv[args_index];
			args_index += 1;
			patterns.push_back(pattern);
		}
		vector<string> aux(0);
		for(string pattern : patterns){
			if(edit_num >= (int)pattern.size()){
				printf("pmt: The pattern \"%s\" must be bigger than the edit number...\n", (char*)pattern.c_str());
				continue;
			}
			aux.push_back(pattern);
		}
		patterns = aux;
		aux.clear();
		
		for(int i = args_index;i<argc;i++){
			textfiles.push_back(argv[i]);
		}
	} else {
		PrintHelp();
		exit(0);
	}
}

void chooseAlgorithm(){
	if(algorithm == Undefined){
		if(!edit_num){
			if(patterns.size() < 3)
				algorithm = Shift_Or;
			else
				algorithm = Aho_Corasick;
		}
	}
}

void runPMT(){
	long** C[patterns.size()];
	bool flag = false;
	long occ[patterns.size()];
	long occ_count[patterns.size()];
	tuple< unordered_map<int, int>, vector<int> , set<int> >ukkonen_fsm[patterns.size()];
	tuple< unordered_map< int ,int>, vector<int> , unordered_map< int ,int>, vector<int>, unordered_map<int, set<int> >> aho_corasick_fsm;
	
	for(unsigned int i=0; i < patterns.size(); i++) {
		C[i] = NULL;
		occ[i] = 0;
		occ_count[i] = 0;
	}
	
	if(algorithm != Aho_Corasick){ 
		int i = 0;
		for(string pattern_string : patterns){
			if((algorithm == Shift_Or) || (algorithm == Wu_Manber)){
				C[i] = buildMasks((char*)pattern_string.c_str());
			} else {
				ukkonen_fsm[i] = build_ukkonen_fsm(pattern_string, edit_num);
			}
			i++;
		}

		for(string file_string : textfiles){
			ifstream textfile;
			textfile.open(file_string);
			if(!textfile.is_open()){
				printf("pmt: %s: Text file doesn't exist\n", (char*)file_string.c_str());
				continue;
			}
				
			string line;

			memset(occ, 0ll, sizeof(occ));
			memset(occ_count,0ll, sizeof(occ_count));

			while(getline(textfile, line)){
				flag = false;
				int i = 0;
				for(string pattern : patterns) {
					//define algorithm by pattern
					if(algorithm == Undefined){
						if(pattern.size() <= 50)
							algorithm = Ukkonen;
						else
							algorithm = Wu_Manber;
					}

					switch(algorithm){
						case Shift_Or:
							if((occ[i] = ShiftOr((char*)pattern.c_str(), (char*)line.c_str(), C[i])))
								flag = true;
							break;
						case Wu_Manber:
							if((occ[i] = WuManber((char*)pattern.c_str(), (char*)line.c_str(), C[i], edit_num)))
								flag = true;
							break;
						case Ukkonen:
							if((occ[i] = ukkonen((char*)line.c_str(), 0, get<0>(ukkonen_fsm[i]), get<1>(ukkonen_fsm[i]), get<2>(ukkonen_fsm[i]))))
								flag = true;
							break;
						default:
							break;
					}
					occ_count[i] += occ[i];
					i++;
				}
				if(flag && !count_occ){
					if(textfiles.size() > 1){
						printf("%s: ", (char*)file_string.c_str());
					}
					printf("%s\n", (char*)line.c_str());
				}
			}
			if(count_occ){
				long sum = 0ll;
				if(textfiles.size() > 1){
					printf("%s: ", (char*)file_string.c_str());
				}
				for(unsigned int i=0; i < patterns.size(); i++) {
					sum += occ_count[i];
					printf("(%s %ld) ", (char*)patterns[i].c_str(), occ_count[i]);
				}
				if(patterns.size() > 0)
					printf("= (%ld)\n", sum);
			}
			textfile.close();
		}
		for(unsigned int i=0; i < patterns.size(); i++) {
			if(C[i]!=NULL)
				delete [] C[i];
		}

	} else {
		aho_corasick_fsm = build_aho_corasick(patterns);
		
		for(string file_string : textfiles){
			ifstream textfile;
			textfile.open(file_string);
			if(!textfile.is_open()){
				printf("pmt: %s: Text file doesn't exist\n", (char*)file_string.c_str());
				continue;
			}
			string line;

			vector<long> occ_count(patterns.size(), 0);

			while(getline(textfile, line)){
				flag = false;
				if(!count_occ){
					flag = aho_corasick((char*)line.c_str(), 0, get<0>(aho_corasick_fsm), get<1>(aho_corasick_fsm), get<2>(aho_corasick_fsm), get<3>(aho_corasick_fsm), get<4>(aho_corasick_fsm));
				} else {
					aho_corasick((char*)line.c_str(), 0, occ_count, get<0>(aho_corasick_fsm), get<1>(aho_corasick_fsm), get<2>(aho_corasick_fsm), get<3>(aho_corasick_fsm), get<4>(aho_corasick_fsm));
				}
				if(flag) {
					if(textfiles.size() > 1){
						printf("%s: ", (char*)file_string.c_str());
					}
					printf("%s\n", (char*)line.c_str());
				}
			}
			if(count_occ){
				long sum = 0ll;
				if(textfiles.size() > 1){
					printf("%s: ", (char*)file_string.c_str());
				}
				for(unsigned int i=0; i<occ_count.size(); i++) {
					sum += occ_count[i];
					printf("(%s %ld) ", (char*)patterns[i].c_str(), occ_count[i]);
				}
				printf("= (%ld)\n", sum);
			}
			textfile.close();
		}
	}
}

int main(int argc, char **argv){
	try {
		ProcessOptions(argc, argv);
		ProcessParameters(argc, argv);
		chooseAlgorithm();
		runPMT();
	} catch (const char* msg){
		PrintHelp();
		cout << "error: " << msg << endl;
		exit(0);
	}

	return 0;
}
