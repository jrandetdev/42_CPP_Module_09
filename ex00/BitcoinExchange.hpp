#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>

# define MINYEAR 2009
# define MAXYEAR 2026

struct InputData
{
	std::string date;
	float		value;
};

class	BitcoinExchange
{
	private:
		std::map<std::string, std::string> historicalData;
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();
};

#endif
