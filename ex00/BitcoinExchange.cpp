#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() : historicalData()
{}

BitcoinExchange::BitcoinExchange(const std::string& fileName)
{
	std::string date, valueStr;
	std::string line;
	std::ifstream fileStream(fileName.c_str());
	getline(fileStream, line);
	if (!isValidExtension(fileName, "csv") || !isFileOpen(fileStream) || !isValidFirstLine(line, "date,exchange_rate"))
		throw std::runtime_error("Error: Invalid file: " + fileName);
	while (fileStream.peek() != EOF)
	{
		std::getline(fileStream, date, ',');
		std::getline(fileStream, valueStr);
		float value = getFloat(valueStr);
		historicalData.insert(std::pair<std::string,float>(date, value));
	}
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : historicalData(other.historicalData) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
	{
		this->historicalData = other.historicalData;
	}
	return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

float	BitcoinExchange::getBitcoinPriceatDate(const std::string& date, float value)
{
	std::map<std::string, float>::iterator it;
	it = this->historicalData.find(date);

	// upperbound finds the first element strictly greater than the key asked for (lower bound is >= greater or equal to)
	it = this->historicalData.upper_bound(date);
	// this is in case the date is earlier but this check should never happen because we did a minyear check
	if (it == this->historicalData.begin())
		return (-1.0);
	// in all cases, it will go back 1. so if our date is missing, then it will give the one right before that one
	--it;
	return (it->second * value);
}
