#include <iostream>

#include <cstdio> // printf

#include <sstream>
#include <iomanip>

#include <bitset> // cin numbers in bits or convert int to bits

#include <algorithm> //std::transform
using namespace std;


template< typename T >
string int_to_hex_up( T i )
{
	stringstream stream;
	stream  << "0x"
			<< std::setfill('0') << std::setw(sizeof(T)*2)
			<< std::uppercase //
			<< std::hex << i;
	return stream.str();
}

template< typename T >
string int_to_hex( T i )
{
	stringstream stream;
	stream  << "0x"
			<< std::setfill('0') << std::setw(sizeof(T)*2)
			<< std::hex << i;
	return stream.str();
}

template< typename T >
string int_to_oct( T i )
{
	stringstream stream;
	stream  << std::setfill('0') << std::setw(sizeof(T)*2)
			<< std::oct << i;
	return stream.str();
}

// http://stackoverflow.com/questions/313970/how-to-convert-stdstring-to-lower-case
char easytolower(char in){
	if(in<='Z' && in>='A')
		return in-('Z'-'z');
	return in;
}

char easytoupper(char in){
	if(in<='z' && in>='a')
		return in+('Z'-'z');
	return in;
}

int main(){
	int start = 50, end = 70;

	cout << "1.: display ints using printf\n";
	for (int i=start; i!=end; ++i){
		printf("-int: %i, hex: 0x%02X, oct: %05o, char: %c\n", i, i, i, i);
	}


	cout << "\n2.: Now in C++\n";
	for (int i=start; i!=end; ++i){
		cout << "i: "               << i
			 << ", hex: "           << int_to_hex(i)
			 << ", hex capitals: "  << int_to_hex_up(i)
			 << ", oct: "           << int_to_oct(i)
			 << ", bits: "          << std::bitset<8>(i) // http://stackoverflow.com/questions/8222127/changing-integer-to-binary-string-of-digits
			 << ", char: "          << (char) i
			 << endl;
	}


	cout << "\n\n3.: setprecision to display PI\n";
	double f =13.14159;

	cout << "3 digits: " << std::setprecision(3) << f << endl;
	cout << "9 digits: " << std::setprecision(9) << f << endl;

	cout << "\nNow Fix number of digits after .\n";
	std::cout << std::fixed;
	cout << "3 digits: " << std::setprecision(3) << f << endl;
	cout << "9 digits: " << std::setprecision(9) << f << endl;



	cout << "\n4.: Number format (default/fixed/scientic)\n";
	double a = 3.1415926534;
	double b = 2006.0;
	double c = 1.0e-10;

	cout.precision(5);

	cout << "default:\n";
	cout << a << '\n' << b << '\n' << c << '\n';

	cout << "\nfixed:\n" << std::fixed;
	cout << a << '\n' << b << '\n' << c << '\n';

	cout << "\nscientific:\n" << std::scientific;
	cout << a << '\n' << b << '\n' << c << '\n';



	cout << "\n\n5.: String to upper or lower case:\n";

	string src = "Test String for FUUUN";
	cout << "Source: " << src << endl;

	string data = src;
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	cout << "Tolower: " << data << endl;

	std::transform(data.begin(), data.end(), data.begin(), ::easytoupper);
	cout << "Easy to upper: " << data << endl;

	//http://www.cplusplus.com/reference/cctype/tolower/
	cout << "cctype tolower with for loop: ";
	for (int i=0; i < src.length(); ++i){
		cout << (char) tolower(src[i]);
	}
	cout << endl;

	// C++ std::tolower from locale: http://www.cplusplus.com/reference/locale/tolower/
	cout << "C++ std::tolower from locale with iterator: ";
	for (string::iterator it = src.begin(); it != src.end(); ++it){
		std::locale loc;
		cout << std::tolower(*it, loc);
	}
	cout << endl;

	return 1;
}
