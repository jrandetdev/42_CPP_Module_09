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
std::string	trimWhiteSpaces(std::string s1);
bool		isValidPrice(const std::string& price);
void	isValidDateSynthax(std::string date);

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

bool	isValidFileExtension(const std::string& file)
{
	std::string strFile = file;
	if ((strFile.substr(strFile.find_last_of(".") + 1 )) != "txt")
	{
		std::cerr << "Error: file needs .txt extension." << std::endl;
		return false;
	}
	return true;
}

// csv file: date exchange rate
// input.txt: date price 
// read the input.txt line by line, validate the date, synthax and price (positive integer and float)
// then the container is the one to actually store the csv file, not input.txt.
bool readInputFile(const std::string& filename, char separatingCharacter)
{
	std::ifstream file(filename.c_str());

	if (!file.is_open()) { std::cerr << "Unable to open the config file: " + filename << std::endl; return false; }
	std::string line;

	while (std::getline(file, line))
	{
		if (line.empty())
			continue ;
		
		// Find the position of the '|' character
		size_t separatorPos = line.find(separatingCharacter);
		if (separatorPos == line.npos) { std::cerr << "Error: bad input => " + line << std::endl; continue; }
		std::string key = trimWhiteSpaces(line.substr(0, separatorPos));
		std::string value = trimWhiteSpaces(line.substr(separatorPos + 1));

		// Check if key or value is empty
		if (key.empty()) { std::cerr << "Error: bad input => " + line << std::endl; continue; }
		if (value.empty()) { std::cerr << "Error: bad input => " + line << std::endl; continue; }
		
		// Check if input is valid (date and number)
		isValidDateSynthax(key);
		//if (!isValidDate(key)) { std::cerr << "Error: bad input => " + line << std::endl; continue; }
		if (isValidPrice(value)) { continue; }
		
		// Insert the values in the map container
	}
	return (true);
}

std::string	trimWhiteSpaces(std::string s1)
{
	s1.erase(0, s1.find_first_not_of(" \t")); // [first, last(
	s1.erase(s1.find_last_not_of(" \t") + 1);
	return s1;
}

void	isValidDateSynthax(std::string date)
{
	// size_t firstHyphenPosition = date.find('-');
	// size_t secondHypenPosition = date.find('-', firstHyphenPosition + 1);

	// if (firstHyphenPosition == date.npos || secondHypenPosition == date.npos)
	// 	return false;
	
	// std::string year = date.substr(0, firstHyphenPosition);
	// std::string month = date.substr(firstHyphenPosition + 1, secondHypenPosition - firstHyphenPosition);
	// std::string day = date.substr(secondHypenPosition + 1);

	// if (date.find('-', secondHypenPosition + 1) != date.npos)
	// 	return (false);

	int year, month, day;
	char first, second;
	std::stringstream ss(date);

	ss >> year >> first >> month >> second >> day;
	if (first != '-' || second != '-')
		return ; 
	// what checks do I need to do for the first year and month to exclude other possibilities
	if (!(MINYEAR <= year && year <= MAXYEAR) || !(1 <= month && month <= 12) || !(1 <= day && day <= 31))
		return ;
	
	std::cout << "Date is valid " << year << first << month << second << day << std::endl; // this is just for testing 
}

bool	isValidPrice(const std::string& price)
{
	(void)price;
	return (true);
}

