#include "BitcoinExchange.hpp"
#include "Parsing.hpp"

#include <iomanip>
#include <exception>

static bool readAndTreatData(const std::string& filename, BitcoinExchange& btcData);

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: could not open a file." << std::endl;
		return 1;
	}

	// BitcoinExchange class is abstracted, all its checks are done separately in the constructor 
	try 
	{
		BitcoinExchange btcdata("data.csv"); // if exception is caught, control is passed to catch block
		if (!readAndTreatData(argv[1], btcdata))
			return 1;
	}
	catch (std::exception& e)
	{
		std::cout << "Standard exception: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}


static bool readAndTreatData(const std::string& filename, BitcoinExchange& btcData)
{
	(void)btcData;
	std::string 	line;
	std::ifstream	file(filename.c_str());
	std::string 	date, valueStr;
	float			value;

	if (!isValidExtension(filename, "txt") || !isFileOpen(file))
		return false;
	
	if (!std::getline(file, line) || !isValidFirstLine(line, "date | value"))
		return false;

	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		size_t pos = line.find('|');
		if (pos == line.npos) {
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		// Extract the date and value
		date = line.substr(0, pos - 1);
		valueStr = line.substr(pos + 1);

		// Check the date
		if (date.length() != 10 || !isValidDate(date)) //|| date[4] != '-' || date[7] != '-' || !isValidDate(date))
		{
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}

		// Check the float value 
		value = getFloat(valueStr);
		if (!isValidValue(value))
			continue;

		// Get the price of the amount of bitcoin at that date
		float result = btcData.getBitcoinPriceatDate(date, value);
		if (result >= 0)
			std::cout << date << " => " << value << " = " << result << std::endl;
	}

	return true;
}
