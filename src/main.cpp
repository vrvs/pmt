#include <iostream>
#include <getopt.h>
#include <vector>

using namespace std;

string algorithm_name = "";
string patternfile = "";
int occ_count = -1;
int edit_num = 0;
string pattern = "";
vector<string> textfiles = vector<string>(0);

void PrintAlgorithms(){
	cout << "alg1" << endl;
	cout << "alg2" << endl;
	cout << "alg3" << endl;
	cout << "alg4" << endl;
}

int ProcessOptions(int argc, char **argv){
	
	int index = 1;
	
	const char* short_opts = "e:p:a:c:h";
	const option long_opts[] = {
		{"edit", required_argument, nullptr, 'e'},
		{"pattern", required_argument, nullptr, 'p'},
		{"algorithm", required_argument, nullptr, 'a'},
		{"count", required_argument, nullptr, 'c'},
		{"help", no_argument, nullptr, 'h'},
		{nullptr, no_argument, nullptr, 0}
	};
	
	while(true){
		const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
		
		if(opt == -1) break;
		
		switch(opt){
			case 'e':
				edit_num = stoi(optarg);
				cout << "edit number: " << edit_num << endl;
				break;
			case 'p':
				patternfile = optarg;
				cout << "pattern file: " << patternfile << endl;
				break;
			case 'a':
				algorithm_name = optarg;
				// TODO Verify if it's a valid algorithm name				
				cout << "using algorithm: " << algorithm_name << endl;
				break;
			case 'c':
				occ_count = stoi(optarg);
				cout << "occurence count: " << occ_count << endl; 
				break;
			case 'h':
				PrintAlgorithms();
				break;
			default:
				break;
		}
		
		index+=2;
	}
	return index;
}

void ProcessParameters(int argc, char** argv, int index){
	pattern = argv[index++];
	
	for(int i = index;i<argc;i++){
		textfiles.push_back(argv[i]);
	}
	
	cout << "The pattern is: " << pattern << endl;
	cout << "The text files are: " << endl; 
	for(auto it = textfiles.begin();it!=textfiles.end();it++){
		cout << "- " << *it << endl;
	}
}

int main(int argc, char **argv){
	int index = ProcessOptions(argc, argv);
	ProcessParameters(argc, argv, index);
	return 0;
}
