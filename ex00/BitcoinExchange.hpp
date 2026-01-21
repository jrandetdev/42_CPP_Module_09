#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>

# define MINYEAR 2009
# define MAXYEAR 2026

struct Date
{
	int y;
	int m;
	int d;
};

// 
bool		isValidLine(std::string& line, char separatingCharacter);
std::string	trimWhiteSpaces(std::string s1);


bool		isValidPrice(const std::string& price);
bool		isValidDate(Date *datestruct);
bool		isLeap(int year);

#endif
