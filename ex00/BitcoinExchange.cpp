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

float	BitcoinExchange::getBitcoinPriceatDate(InputData *input)
{
	std::map<std::string, float>::iterator it;
	it = this->historicalData.find(input->date);

	// upperbound finds the first element strictly greater than the key asked for (lower bound is >= greater or equal to)
	it = this->historicalData.upper_bound(input->date);
	// this is in case the date is earlier but this check should never happen because we did a minyear check
	if (it == this->historicalData.begin())
		return (-1.0);
	// in all cases, it will go back 1. so if our date is missing, then it will give the one right before that one
	--it;
	return (it->second * input->value);
}
