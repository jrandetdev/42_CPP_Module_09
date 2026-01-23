#include "BitcoinExchange.hpp"
#include "Parsing.hpp"

#include <iomanip>

static bool readAndTreatData(const std::string& filename, const BitcoinExchange& btcData);

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: could not open a file." << std::endl;
		return 1;
	}

	// Check file and extract data in container using the constructor which takres an ifstream
	std::ifstream csvFileStream("data.csv");
	if (!isValidFile("data.csv", csvFileStream, "csv"))
		return 1;
	BitcoinExchange btcdata(csvFileStream);
	btcdata.printMapElement();
	readAndTreatData(argv[1], btcdata);
	return (0);
}


static bool readAndTreatData(const std::string& filename, const BitcoinExchange& btcData)
{
	(void)btcData;
	std::ifstream	file(filename.c_str());
	SeparatedTokens elements;
	InputData		input;
	
	std::string line;
	if (!isValidFile(filename, file, "txt") || !isValidFirstLine(file, line))
		return false;

	while (file.peek() != EOF)
	{
		getline(file, line);
		if (line.empty())
			continue ;

		std::stringstream ss(line);
		if (!(ss >> elements.year >> elements.firstHyphen
			>> elements.month >> elements.secondHyphen
			>> elements.day >> elements.verticalBar
			>> input.value) || elements.firstHyphen != '-' ||
			elements.secondHyphen != '-' || elements.verticalBar != '|')
		{
			std::cout << "Error: bad input => " + line << std::endl;
			continue;
		}

		// Check negative value to have the correct error message 
		if (!isValidValue(input.value))
			continue;

		// Check the date
		if (!isValidDate(&elements))
		{
			std::cout << "Error: bad input => " + line << std::endl;
			continue;
		}

		// Get the date for searching the map
		extractDateKey(&elements, &input);
		ss.str("");
		ss.clear();
		

	}
	return true;
}
