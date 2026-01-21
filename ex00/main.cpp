#include "BitcoinExchange.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>

bool		isValidFileExtension(const std::string& file);
bool 		readInputFile(const std::string& filename, char separatingCharacter);

int	main(int argc, char **argv)
{
	(void)argv;
	if (argc != 2)
	{
		std::cerr << "Error: could not open a file." << std::endl;
		return 1;
	}
	std::string strFile = argv[1];
	isValidFileExtension(strFile);
	readInputFile(strFile, '|');
	std::cout << "finished the while loop" << std::endl;
}


bool readInputFile(const std::string& filename, char separatingCharacter)
{
	// Check for .txt extension
	if (!isValidFileExtension(filename))
	{
		std::cerr << "Error: could not open file." << std::endl;
		return false;
	}
	
	// Check if filr is open before starting
	std::ifstream file(filename.c_str());
	if (!file.is_open()) { std::cerr << "Error: could not open file." << std::endl; return false; }
	
	std::string line;
	
	// Reading protocol to check line by line (no ending the program if an error is found)
	while (std::getline(file, line))
	{
		if (line.empty())
			continue ;
		
		if (!(isValidLine(line, separatingCharacter)))
		{
			std::cerr << "Error: bad input => " + line << std::endl;
			continue;
		}
		// // Insert the values in the map container
	}
	return (true);
}

bool	isValidFileExtension(const std::string& file)
{
	if ((file.substr(file.find_last_of(".") + 1 )) != "txt")
		return false;
	return true;
}

bool	isValidLine(std::string& line, char separatingCharacter)
{
	Date dateStruct;

	if (line.empty())
			return false;

	std::stringstream ss(line);
	float bitcoinAmount;
	char firstDateHyphen, secondDateHyphen, separation;

	ss >> dateStruct.y >> firstDateHyphen >> dateStruct.m >> secondDateHyphen >> dateStruct.d >>
		separation >> bitcoinAmount;
	
	//  Check separators 
	if (firstDateHyphen != '-' || secondDateHyphen != '-' ||
		separation != separatingCharacter)
		return false;
	
	// Check if input is valid (date and number)
	if (!isValidDate(&dateStruct) || !isValidExchangeRate(bitcoinAmount))
		return false;
	
	return true;
}


bool	isValidDate(Date *datestruct)
{
	// Check year and month are in the right range
	if (!(MINYEAR <= datestruct->y && datestruct->y <= MAXYEAR) ||
		!(1 <= datestruct->m && datestruct->m <= 12) ||
		!(1 <= datestruct->d && datestruct->d <= 31))
	{
		std::cout << "first date check failed" << std::endl;
		return false;
	}
	
	// Handle february month with leap year
	if (datestruct->m == 2)
	{
		if (isLeap(datestruct->y))
			return (datestruct->d <= 29);
		else
			return (datestruct->d <= 28);
	}
	
	// Handle april june september have  30 days max
	if (datestruct->m == 4 || datestruct->m == 6 ||
		datestruct->m == 9 || datestruct->m == 11)
		return (datestruct->d <= 30);
		
	return true;
}
	
bool	isLeap(int year)
{
	return (((year % 4 == 0) &&
	(year % 100 != 0)) ||
	(year % 400 == 0));
}

bool	isValidExchangeRate(float bitcoinAmount)
{
	if (bitcoinAmount <= 0 || bitcoinAmount >= 1000)
		return false;
	
	return (true);
}

std::string	trimWhiteSpaces(std::string s1)
{
	s1.erase(0, s1.find_first_not_of(" \t")); // [first, last(
	s1.erase(s1.find_last_not_of(" \t") + 1);
	return s1;
}
