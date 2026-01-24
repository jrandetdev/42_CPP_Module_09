#include "BitcoinExchange.hpp"
#include "Parsing.hpp"

#include <iomanip>

static bool readAndTreatData(const std::string& filename, BitcoinExchange& btcData);

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: could not open a file." << std::endl;
		return 1;
	}

	// Check file and extract data in container using the constructor which takres an ifstream
	std::ifstream csvFileStream("bad_data.csv");
	if (!isValidFile("bad_data.csv", csvFileStream, "csv") || 
		!isValidFirstLine("bad_data.csv", csvFileStream, "date,exchange_rate"))
		return 1;
	BitcoinExchange btcdata(csvFileStream);
	//btcdata.printMapElement();
	readAndTreatData(argv[1], btcdata);
	return (0);
}


static bool readAndTreatData(const std::string& filename, BitcoinExchange& btcData)
{
	(void)btcData;
	std::ifstream	file(filename.c_str());
	SeparatedTokens elements;
	InputData		input;
	
	if (!isValidFile(filename, file, "txt") || !isValidFirstLine(filename, file, "date | value"))
	return false;
	std::string line;

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
			elements.secondHyphen != '-' || elements.verticalBar != '|' ||
			!isValidDate(&elements))
		{
			std::cout << "Error: bad input => " + line << std::endl;
			continue;
		}

		// Check negative value to have the correct error message 
		if (!isValidValue(input.value))
			continue;

		// Get the date for searching the map
		extractDateKey(&elements, &input);
		float result = btcData.getBitcoinPriceatDate(&input);
		//std::cout << "result is " << result << std::endl;
		if (result == -1.0)
			continue;
		std::cout << input.date << " => " << input.value << " = " << result << std::endl;
		ss.str("");
		ss.clear();
	}
	return true;
}
