#include "BitcoinExchange.hpp"
#include "Parsing.hpp"

bool	isValidFile(const std::string& filename, std::ifstream& file, const std::string& extension)
{
	// Check if file is open (expensive system call, do it once)
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return false;
	}
	
	// Checking hte extension to see if it is correct
	size_t dotPos = filename.find_last_of('.');
	if (filename.substr(dotPos + 1) != extension)
	{
		std::cerr << "Error: Invalid file extension. Expected ." << extension << std::endl;
		return false;
	}
	return true;
}

bool	isValidFirstLine(std::ifstream& file, std::string& line)
{
	// Check first line is date | value as asked by the subject
	getline(file, line);
	if (line != "date | value")
	{
		std::cerr << "Error: file needs to start with date | value" << std::endl;
		return false;
	}
	return true;
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


bool	isValidDate(SeparatedTokens *elements)
{
	// Check year and month are in the right range
	if (!(MINYEAR <= elements->year && elements->year <= MAXYEAR) ||
	!(1 <= elements->month && elements->month <= 12) ||
	!(1 <= elements->day && elements->day <= 31))
	{
		return false;
	}
	
	// Handle february month with leap year
	if (elements->month == 2)
	{
		if (isLeap(elements->year))
			return (elements->day <= 29);
		else
			return (elements->day <= 28);
	}
	
	// Handle april june september have  30 days max
	if (elements->month == 4 || elements->month == 6 ||
		elements->month == 9 || elements->month == 11)
		return (elements->day <= 30);
		
	return true;
}
	
bool	isLeap(int year)
{
	return (((year % 4 == 0) &&
	(year % 100 != 0)) ||
	(year % 400 == 0));
}
	
void	extractDateKey(SeparatedTokens *elements, InputData *input)
{
	std::stringstream ss;

	ss << std::setfill('0') << std::setw(4) << elements->year
		<< std::setw(2) << elements->month
		<< std::setw(2) << elements->day;

	ss >> input->date;
}
