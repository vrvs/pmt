#include <iostream>
#include <fstream>
#include <getopt.h>
#include <vector>
#include "algorithms.h"

using namespace std;


string algorithm_name = "";
string patternfile = "";
int occ_count = -1;
int edit_num = 0;
string pattern = "";
vector<string> textfiles = vector<string>(0);

void PrintAlgorithms(){
	cout << "ShiftOr" << endl;
	cout << "AhoCorasick" << endl;
	cout << "WuManber" << endl;
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
		else if(opt == 'h') index++;
		else index+=2;
		
		switch(opt){
			case 'e':
				try {
					edit_num = stoi(optarg);
				} catch (const exception &err) {
					throw "The edit number must be an integer.";
				}

				if(edit_num < 0){
					throw "The edit number can't be less than zero.";
				}

				cout << "edit number: " << edit_num << endl;
				break;

			case 'p':
				patternfile = optarg;
				cout << "pattern file: " << patternfile << endl;
				break;

			case 'a':
				algorithm_name = optarg;
				if(algorithm_name.compare("ShiftOr") && algorithm_name.compare("AhoCorasick") && 
					algorithm_name.compare("WuManber") && algorithm_name.compare("alg4")){
					PrintAlgorithms();
					exit(0);
				}
				cout << "using algorithm: " << algorithm_name << endl;
				break;
				
			case 'c':
				try {
					occ_count = stoi(optarg);
				} catch (const exception &err) {
					throw "The occurence count must be an integer.";
				}
				if(occ_count < 0){
					throw "The occurence count can't be less than zero.";
				}

				cout << "occurence count: " << occ_count << endl; 
				break;

			case 'h':
				PrintAlgorithms();
				exit(0);
				break;

			default:
				break;
		}
	}
	return index;
}

void ProcessParameters(int argc, char** argv, int index){
	if(index+1 < argc){
		pattern = argv[index++];
	
		for(int i = index;i<argc;i++){
			textfiles.push_back(argv[i]);
		}
		
		/*cout << "The pattern is: " << pattern << endl;
		cout << "The text files are: " << endl; 
		for(auto it = textfiles.begin();it!=textfiles.end();it++){
			cout << "- " << *it << endl;
		}*/
	} else {
		//TODO raise error "there isnt enough arguments"
		cout << "There isn't enough arguments" << endl;	
	}
}

void chooseAlgorithm(){
}

int main(int argc, char **argv){
	try {
		int index = ProcessOptions(argc, argv);
		ProcessParameters(argc, argv, index);
		chooseAlgorithm();
	} catch (const char* msg){
		cout << msg << endl;
	}

	ifstream file1(textfiles[0]);
	string text;
	
	long** C = buildMasks((char*)pattern.c_str());

	while(getline(file1, text)){

		// need to use switch????
		if(!algorithm_name.compare("ShiftOr")){
			if(ShiftOr((char*)pattern.c_str(), (char*)text.c_str(), C))
				cout << text << endl;
		} else if(!algorithm_name.compare("WuManber")){
			if(Wu_Manber((char*)pattern.c_str(), (char*)text.c_str(), C, edit_num))
				cout << text << endl;
		}
		
	}

	//Shiftor((char*)"arranhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aar", (char*)"a arranhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aarranhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aar areeee");
	return 0;
}
