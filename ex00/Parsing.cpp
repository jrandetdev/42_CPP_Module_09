#include "BitcoinExchange.hpp"
#include "Parsing.hpp"

bool	isValidExtension(const std::string& filename, const std::string& extension)
{
	// Checking hte extension to see if it is correct
	size_t dotPos = filename.find_last_of('.');
	if (filename.substr(dotPos + 1) != extension)
	{
		std::cerr << "Error: Invalid file extension. Expected ." << extension << std::endl;
		return false;
	}
	return true;
}

bool	isFileOpen(std::ifstream& file)
{
	// Check if file is open (expensive system call, do it once)
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return false;
	}
	return true;
}

bool	isValidFirstLine(std::string& line, const std::string& firstLine)
{
	// Check first line is date | value as asked by the subject
	if (line != firstLine)
		return false;
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

bool	isValidValue(float value)
{
	if (value < 0)
	{
		std::cout << "Error: not a positive number." << std::endl;
		return false;
	}
	else if (value > 1000) 	// Check too large a number
	{
		std::cout << "Error: too large a number." << std::endl;
		return false;
	}
	return true;
}


bool	isValidDate(const std::string& date)
{
	int year, month, day;

	if (date[4] != '-' || date[7] != '-')
		return false;

	size_t firstPos = date.find('-');
	size_t secondPos = date.find_last_of('-');

	year = atoi((date.substr(0, firstPos)).c_str());
	month = atoi((date.substr(firstPos + 1, 2)).c_str());
	day = atoi((date.substr(secondPos + 1, 2)).c_str());

	// Check year and month are in the right range
	if (!(MINYEAR <= year && year <= MAXYEAR) ||
	!(1 <= month && month <= 12) ||
	!(1 <= day && day <= 31))
	{
		return false;
	}
	
	// Handle february month with leap year
	if (month == 2)
	{
		if (isLeap(year))
			return (day <= 29);
		else
			return (day <= 28);
	}
	
	// Handle april june september have  30 days max
	if (month == 4 || month == 6 ||
		month == 9 || month == 11)
	{
		return (day <= 30);
	}

	return true;
}
	
bool	isLeap(int year)
{
	return (((year % 4 == 0) &&
	(year % 100 != 0)) ||
	(year % 400 == 0));
}
