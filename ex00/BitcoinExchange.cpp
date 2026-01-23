#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() : historicalData()
{}

BitcoinExchange::BitcoinExchange(std::ifstream& csvFile)
{
	std::string date, valueStr;
	char *end = NULL;
	while (csvFile.peek() != EOF)
	{
		std::getline(csvFile, date, ',');
		std::getline(csvFile, valueStr);
		float value = std::strtof(valueStr.c_str(), &end);
		historicalData.insert(std::pair<std::string,float>(date, value));
	}
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : historicalData(other.historicalData)
{

}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
	{
		this->historicalData = other.historicalData;
	}
	return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::printMapElement()
{
	std::cout << "1010-08-20 contains " << historicalData.at("2010-08-20") << std::endl;
}
