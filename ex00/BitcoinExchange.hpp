#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <exception>
#include "Parsing.hpp"

# define MINYEAR 2009
# define MAXYEAR 2026


class	BitcoinExchange
{
	private:
		std::map<std::string, float> historicalData;
	public:
		// Canonical form
		BitcoinExchange();
		BitcoinExchange(const std::string& fileName);
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();

		float	getBitcoinPriceatDate(const std::string& date, float value);
};

#endif
