#include "BitcoinExchange.hpp"
#include "Parsing.hpp"

#include <iomanip>
#include <exception>

static bool readAndTreatData(const std::string& filename, BitcoinExchange& btcData);

int	main(int argc, char **argv)
{
	if (argc != 2) { std::cerr << RED << "Error: could not open a file." << RESET << std::endl; return 1; }

	try {
		BitcoinExchange btcdata("data.csv");
		if (!readAndTreatData(argv[1], btcdata)) return 1;
	}
	catch (std::exception& e) {
		std::cout << "Standard exception: " << e.what() << RESET << std::endl; return 1;
	}
	return 0;
}


static bool readAndTreatData(const std::string& filename, BitcoinExchange& btcData)
{
	std::string 	line;
	std::ifstream	file(filename.c_str());

	if (!isValidExtension(filename, "txt") || !isFileOpen(file)) return false;
	if (file.peek() == EOF) { std::cerr << RED << "Error: File is empty." << RESET << std::endl; return false; }
	// Check the first line of the file
	if (!std::getline(file, line) || !isValidFirstLine(line, "date | value")) return false;

	// Variables and stream necessary for the parsing of the input file
	int year, month, day;
	char firstHyphen, secondHyphen, verticalBar;
	float value;
	std::string date;

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		if (line.empty()) continue;

		if (!(ss >> year >> firstHyphen >> month >> secondHyphen >> day >> verticalBar >> value) ||
			firstHyphen != '-' || secondHyphen != '-' || verticalBar != '|' || !isValidDate(year, month, day))
		{
			std::cerr << RED << "Error: bad input => " << line << RESET << std::endl; continue;
		}

		// Check the float value 
		if (!isValidValue(value)) continue;

		size_t delimPos = line.find(verticalBar);
		date = line.substr(0, delimPos);

		// Get the price of the amount of bitcoin at that date
		btcData.getBitcoinPriceatDate(date, value);
	}

	return true;
}
