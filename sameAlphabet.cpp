#include <iostream>
#include <set>
#include <cstring>

using namespace std;

bool hasSameAlphabet(char * refString, char *word){
	// Put all the characters of the reference string in a multiset
	multiset<char> alphabet;
	int len = strlen(refString);
	for (int i=0; i< len; ++i){
		alphabet.insert(refString[i]);
	}

	len = strlen(word);
	for (int i=0; i<len; ++i){
		multiset<char>::iterator it = alphabet.find(word[i]);
		if (it ==  alphabet.end())
			return false;
		else
			alphabet.erase(it);
	}
	return true;
}


int main(int argc, char* argv[]){
	if (argc < 3){
		cout << "Syntax: " << argv[0]
			 << " <reference string> <string to test>+\n";
		return 1;
	}

	char * refString = argv[1];
	cout << "Reference String: " << refString << endl;

	for (int i = 2; i<argc; ++i){
		char * word = argv[i];
		cout << word << ": " << hasSameAlphabet(refString, word) << endl;
	}

	return 0;
}
