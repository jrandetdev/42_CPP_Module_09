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
		//std::cout << "valueStr is worth " << valueStr << std::endl;
		float value = std::strtof(valueStr.c_str(), &end);
		//std::cout << "value is worth " << value << std::endl;
		//std::cout << "inside the constructor the value is " << value << std::endl; 
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

// void BitcoinExchange::printMapElement()
// {
// 	//std::cout << "2010-08-20 contains " << historicalData.at("2010-08-20") << std::endl;
// }

float	BitcoinExchange::getBitcoinPriceatDate(InputData *input)
{
	std::map<std::string, float>::iterator it;
	it = this->historicalData.find(input->date);

	if (it != historicalData.end())
		historicalData.erase(it);

	it = this->historicalData.find(input->date);
	if (it == historicalData.end())
		it = historicalData.upper_bound(input->date);
	return (it->second * input->value);
}
