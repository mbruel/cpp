// g++ rList.cpp -lstdc++fs -std=c++17 -o rList

#include <string>
#include <iostream>
#include <iomanip>    // std::setprecision
#include <experimental/filesystem> // C++-standard header file name
#include <fstream>
#include <algorithm>  // std::replace
namespace fs = std::experimental::filesystem;

static const std::string sOutputFileName = "_list.txt";
static const int         sFileNameLenght = 100;

#define USE_COMA_FLOATS 1

void listFolder(const fs::path &folderPath, std::ofstream &out, int depth = 0, bool isLast = false);
std::string startLine(const fs::path &path, int depth, bool isLast);
bool sortPathWithDirFirst(const fs::path &l, const fs::path &r);
std::string formatSize(unsigned int size);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Error: you should provide exactly one parameter that should be a folder path" << std::endl;
		return 1;
	}

	fs::path folderPath(argv[1]);
	if (!fs::exists(folderPath) || !fs::is_directory(folderPath))
	{
		std::cerr << "Error: the given parameter is not an existing folder" << std::endl;
		return 2;
	}

	fs::path outputFile = folderPath / sOutputFileName;
	std::ofstream output(outputFile.string(), std::ofstream::out | std::ofstream::trunc);
	if (output.is_open())
		listFolder(fs::absolute(folderPath), output);
	else
	{
		std::cerr << "can't write list file in folder.." << std::endl;
		return 2;
	}

	output.close();
	return 0;
}


void listFolder(const fs::path &folderPath, std::ofstream &out, int depth, bool isLast)
{
	if (depth)
		out << startLine(folderPath, depth - 1, isLast) << std::endl;

	std::vector<fs::path> files;

	fs::directory_iterator it = fs::directory_iterator(folderPath), itEnd;
	while (it != itEnd)
	{
		if (it->path().filename().string() != sOutputFileName)
			files.push_back(it->path());
		++it;
	}

	std::sort(files.begin(), files.end(), sortPathWithDirFirst);

//	for (const auto & entry : fs::directory_iterator(folderPath))
//	{
//		const fs::path &path =entry.path();
	int nbFiles = files.size();
	for (int i = 0 ; i < nbFiles ; ++i)
	{
		const fs::path & path = files.at(i);
		if (fs::is_directory(path))
			listFolder(fs::absolute(path), out, depth + 1, i == nbFiles - 1);
		else
		{
			out << std::left  << std::setw(sFileNameLenght) << startLine(path, depth, i == nbFiles - 1)
			    << std::right << std::setw(20) << formatSize(fs::file_size(path)) <<  std::endl;
		}
	}
}

bool sortPathWithDirFirst(const fs::path &l, const fs::path &r)
{
	if (fs::is_directory(l))
	{
		if (fs::is_directory(r))
			return l.filename().string().compare(r.filename().string()) <= 0;
		else
			return true;
	}
	else if (fs::is_directory(r))
		return false;
	else
		return l.filename().string().compare(r.filename().string()) <= 0;
}


std::string startLine(const fs::path &path, int depth, bool isLast){
	std::string start;
	for (int i = 0 ; i < depth; ++i)
		start += "| ";
	start += (isLast ? "└" : "├");
	start += "──── ";
	start += path.filename().string();

	return start;
}

std::string formatSize(unsigned int size)
{
	std::string unit(" B");
	double s = static_cast<double>(size);
	if (s > 1024)
	{
		unit = "kB";
		s /= 1024.;
		if (s > 1024)
		{
			unit = "MB";
			s /= 1024.;
			if (s > 1024)
			{
				unit = "GB";
				s /= 1024.;
			}
		}
	}

	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << s << " " << unit;

#ifdef USE_COMA_FLOATS
	std::string str = ss.str();
	std::replace(str.begin(), str.end(), '.', ',');
	return str;
#else
	return ss.str();
#endif
}

