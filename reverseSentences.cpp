/*
 * The input file has sentences stored as text.
 * Write a program to print the words in the file in reverse order to the screen.
 *
 * g++ reverseSentences.cpp -std=c++11 -o reverseSentences
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
using namespace std;


int main(int argc, char* argv[]){

	if (argc != 2) {
		cout << "Syntax: " << argv[0] << " <text file>\n";
		return 1;
	}

	string fileName{argv[1]};
	ifstream file;
	file.open(fileName, ifstream::in);
	if (!file.good()){
		cout << "can't open file '"
			 << fileName << "' in reading mode\n";
		return 1;
	}


	string line;
	while (std::getline(file, line)){
		istringstream iss(line);
		stack<string> pile;
		string word;

		// store the words of a sentence in a stack
		while (iss >> word){
			pile.push(word);
		}

		// Display the sentence in reverse
		while(!pile.empty()){
			cout << pile.top() << " ";
			pile.pop();
		}
		cout << endl;
	}

	file.close();
	return 0;
}
