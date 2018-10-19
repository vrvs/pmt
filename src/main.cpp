#include <iostream>
#include <fstream>
#include <getopt.h>
#include <vector>
#include "algorithms.h"

using namespace std;

enum Algorithm { Shift_Or, Wu_Manber, Aho_Corasick, Ukkonen, Undefined };

string algorithm_name = "";
Algorithm algorithm = Undefined;
string patternfile = "";
bool count_occ = false;
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
					throw "The edit number must be an integer.";
				}

				if(edit_num < 0){
					throw "The edit number can't be less than zero.";
				}

				//cout << "edit number: " << edit_num << endl;
				break;

			case 'p':
				patternfile = optarg;
				p_file.open(patternfile);
				
				while(getline(p_file,cur_patt))
					patterns.push_back(cur_patt);
				p_file.close();

				//cout << "pattern file: " << patternfile << endl;
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
					PrintHelp();
					exit(0);
				}

				//cout << "using algorithm: " << algorithm_name << endl;
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
	if(args_index+1 < argc){
		pattern = argv[args_index];
		args_index += 1;
		patterns.push_back(pattern);
		
		for(int i = args_index;i<argc;i++){
			textfiles.push_back(argv[i]);
		}
		
		/*cout << "The pattern is: " << pattern << endl;
		cout << "The text files are: " << endl; 
		for(auto it = textfiles.begin();it!=textfiles.end();it++){
			cout << "- " << *it << endl;
		}*/
	} else {
		//TODO raise error "there isnt enough arguments"
		//cout << "There isn't enough arguments" << endl;
		PrintHelp();
		exit(0);
	}
}

void chooseAlgorithm(){
	if(algorithm == Undefined){
		if(patterns.size() == 1){
			if(!edit_num)
				algorithm = Shift_Or;
			else
				algorithm = Wu_Manber;
		} else {
			if(!edit_num)
				algorithm = Aho_Corasick;
			else
				algorithm = Ukkonen;
		}
	}
}

void runPMT(){
	long** C = NULL;
	bool flag = false;
	long occ = 0;
	long occ_count = 0;

	if(algorithm != Aho_Corasick){
		for(string pattern_string : patterns){
			if((algorithm == Shift_Or) || (algorithm == Wu_Manber)){
				C = buildMasks((char*)pattern_string.c_str());
			} else {
				// CONSTRUIR AS COISAS PARA O UKKONEN
			}

			for(string file_string : textfiles){
				ifstream textfile(file_string);
				string line;

				occ = 0;
				occ_count = 0;

				if(!textfile.good()){
					cout << "pmt: " << file_string << ": File doesn't exist" << endl;
					continue;
				}

				while(getline(textfile, line)){
					flag = false;
					switch(algorithm){
						case Shift_Or:
							if((occ = ShiftOr((char*)pattern.c_str(), (char*)line.c_str(), C)))
								flag = true;
							break;
						case Wu_Manber:
							if((occ = WuManber((char*)pattern.c_str(), (char*)line.c_str(), C, edit_num)))
								flag = true;
							break;
						case Ukkonen:
							//COLOCAR AQUI CHAMADA PARA O UKKONEN
							break;
						default:
							break;
					}
					if(flag && !count_occ){
						if(textfiles.size() > 1){
							printf("%s: ", (char*)file_string.c_str());
							//cout << file_string << ": ";
						}
						printf("%s\n", (char*)line.c_str());
						// cout << line << endl;
					}
					occ_count += occ;
				}
				if(count_occ){
					if(textfiles.size() > 1){
						printf("%s: ", (char*)file_string.c_str());
						// cout << file_string << ": ";
					}
					printf("%ld\n", occ_count);
					// cout << occ_count << endl;
				}
			}
			if(C!=NULL)
				delete [] C;
		}
	} else {
		// CONSTROI AS COISAS PARA O AHO COM O patterns
		// ITERA SOBRE CADA ARQUIVO CHAMANDO O AHO PARA CADA LINHA DE CADA ARQUIVO
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

	/*ifstream file1(textfiles[0]);
	string text;
	
	long** C = buildMasks((char*)pattern.c_str()());

	while(getline(file1, text)){

		// need to use switch????
		if(!algorithm_name.compare("ShiftOr")){
			if(ShiftOr((char*)pattern.c_str(), (char*)text.c_str(), C))
				cout << text << endl;
		} else if(!algorithm_name.compare("WuManber")){
			if(WuManber((char*)pattern.c_str(), (char*)text.c_str(), C, edit_num))
				cout << text << endl;
		}
		
	}*/

	return 0;
}
