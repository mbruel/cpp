/*
 * The current time is available in HTML from http://tycho.usno.navy.mil/timer.html
 * Write a program to display the current time (Universal Time) as returned from the site.
 *
 * g++ printUTC.cpp  -std=c++11 -lboost_system -pthread -o printUTC
 */

#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <regex>
using namespace std;


const string url       {"http://tycho.usno.navy.mil/timer.html"};
const string protocol  {"http"};

const string utcLine   {"Universal Time"}; // string used to identify the correct line
const regex  utcRegex  {".*<BR>(\\w{3}\\.\\s+\\d+,\\s+\\d{2}:\\d{2}:\\d{2}\\s+UTC).*"}; //<BR>Nov. 10, 22:45:19 UTC

const regex  urlRegex  {"^(http(s)?://)?([^/]+)/(.*)$"};
enum { buffer_size = 1024, tcp_timeout = 60 };


int main(){

	// Catch the host and the page to get from the url
	std::smatch urlMatch;
	std::regex_match(url, urlMatch, urlRegex);
	if (urlMatch.size() != 5){
		cout << "Error parsing url: " << url << endl;
		return 1;
	}
	string host{urlMatch[3]};
	string page{urlMatch[4]};


	// Create a TCP socket iostream
	boost::asio::ip::tcp::iostream sockStream;

	// Set at timeout for I/O
	sockStream.expires_from_now(boost::posix_time::seconds(tcp_timeout));

	// Try to establish the connection
	sockStream.connect(host, protocol);
	if (!sockStream){
		cout << "Unable to connect to url: " << url << endl;
		cout << "Error: " << sockStream.error().message() << endl;
		return 1;
	}

	// Send a simple Get request for the page
	sockStream << "GET /" << page << "\r\n";

	// Read Answer line by line
	string line;
	while ( std::getline(sockStream, line) ){
		// If the line contains the Universal Time
		if (line.find(utcLine) != string::npos){
			// we try to retrieve it thanks to the utc regex
			std::smatch utcMatch;
			std::regex_match(line, utcMatch, utcRegex);
			if (utcMatch.size() == 2){
				cout << utcMatch[1] << endl;
				break;
			}
		}
	}

	return 0;
}
