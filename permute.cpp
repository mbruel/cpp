#include <iostream>
#include <cstring>
using namespace std;

void permute(char str[], int pos, int last, int *nbPermute){
	if (pos == last){
		cout << str << endl;
		++(*nbPermute);
	}
	else{
		for (int i = pos; i <= last; ++i){
			swap(str[pos], str[i]);
			permute(str, pos+1, last, nbPermute);
			swap(str[pos], str[i]);
		}
	}
}

int main(int argc, char* argv[])
{

	if (argc != 2){
		cout << "Syntax: " << argv[0]
			 << " <string to give the permutations of>\n";
		return 1;
	}

	char *str = argv[1];
	int len = strlen(str);

	int nbPermute = 0;
	permute(str, 0, len-1, &nbPermute);
	cout << "There are " << nbPermute << " different permutations\n";
	return 0;
}
