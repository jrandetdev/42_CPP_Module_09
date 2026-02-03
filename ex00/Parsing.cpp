#include "BitcoinExchange.hpp"
#include "Parsing.hpp"

bool	isValidExtension(const std::string& filename, const std::string& extension)
{
	// Checking hte extension to see if it is correct
	size_t dotPos = filename.find_last_of('.');
	if (filename.substr(dotPos + 1) != extension)
	{
		std::cerr << RED << "Error: Invalid file extension for " << filename <<
			". Expected \"." << extension << "\"." << RESET << std::endl;
		return false;
	}
	return true;
}

bool	isFileOpen(std::ifstream& file)
{
	// Check if file is open (expensive system call, do it once)
	if (!file.is_open())
	{
		std::cerr << RED << "Error: could not open file." << RESET << std::endl;
		return false;
	}
	return true;
}

bool	isValidFirstLine(std::string& line, const std::string& firstLine)
{
	// Check first line of the files is asked as the subject as asked by the subject
	if (line != firstLine)
	{
		std::cerr << RED << "Error: file needs to start with " << firstLine << RESET << std::endl;
		return false;
	}
	return true;
}

float	getFloat(std::string& value)
{
	char *end = NULL;
	float floatTemp = std::strtof(value.c_str(), &end);
	if (end == value || errno == ERANGE)
		return (-1.0);
	return floatTemp;
}

void	validateValue(float value)
{
	if (value < 0)
		throw std::runtime_error("Error: not a positive number.");
	else if (value > 1000)
		throw std::runtime_error("Error: too large of a number.");
}


void	validateDate(int year, int month, int day)
{
	if (!(2009 <= year && year <= 2026))
		throw std::runtime_error("Error: year must be between 2009 and 2026");

	if (!(1 <= month && month <= 12))
		throw std::runtime_error("Error: month must be between 1 and 12");

	if (!(1 <= day && day <= 31)) 
		throw std::runtime_error("Error: day must be between 1 and 30 or 31.");
	
	// Handle february month with leap year
	if (month == 2)
	{
		if (isLeap(year) && day > 29)
			throw std::runtime_error("Error: leap year " + std::to_string(year) + " contains 29 days in February");
		else if (!isLeap(year) && day > 28)
			throw std::runtime_error("Error: " + std::to_string(year) + " is not. leap year and only contains 28 days in February.");
	}
	
	// Handle april june september have  30 days max
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
		throw std::runtime_error("Error: " + std::to_string(month) + " only contains 30 days.");
}
	
bool	isLeap(int year)
{
	return (((year % 4 == 0) &&
	(year % 100 != 0)) ||
	(year % 400 == 0));
}
