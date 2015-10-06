#include <iostream>
using namespace std;

void permute(char str[], int pos, int last){
	if (pos == last)
		cout << str << endl;
	else{
		for (int i = pos; i <= last; ++i){
			swap(str[pos], str[i]);
			permute(str, pos+1, last);
			swap(str[pos], str[i]);
		}
	}
}

int main()
{
	char str[] = "CAT";
	permute(str, 0, 2);
	return 0;
}
