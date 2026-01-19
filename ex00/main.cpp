#include "BitcoinExchange.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <map>

bool	isValidFileExtension(const std::string& file);
void readConfigFile(std::map<std::string, double>& testMap, const std::string& filename);

int	main(int argc, char **argv)
{
	(void)argv;
	if (argc != 2)
	{
		std::cerr << "Error: could not open a file." << std::endl;
		return 1;
	}
	std::string strFile = argv[1];
	isValidFileExtension(strFile);
	std::map<std::string, double> testMap;
	readConfigFile(testMap, strFile);
	std::cout << "finished the while loop" << std::endl;
}

bool	isValidFileExtension(const std::string& file)
{
	std::string strFile = file;
	if ((strFile.substr(strFile.find_last_of(".") + 1 )) != "txt")
	{
		std::cerr << "Error: file needs .txt extension." << std::endl;
		return false;
	}
	return true;
}

void readConfigFile(std::map<std::string, double>& testMap, const std::string& filename)
{
	std::ifstream file(filename.c_str());
	(void)testMap;
	if (!file.is_open())
	{
		std::cerr << "Unable to open the config file: " + filename << std::endl;
		return ;
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
			continue ;
		
		// Find the position of the '|' character
		size_t separatorPos = line.find('|');
		if (separatorPos == line.npos)
		{
			std::cerr << "Invalid config line: " + line << std::endl;
			return ;
		}
		std::string key = line.substr(0, separatorPos);
		std::string value = line.substr(separatorPos + 1);
		std::cout << "before trimming key: " + key << "." << std::endl; 
		std::cout << "before trimming value: " + value << "." << std::endl; 

		// Trim whitespaces
		key.erase(0, key.find_first_not_of(" \t")); // [first, last(
		key.erase(key.find_last_not_of(" \t") + 1); // erases all characters after index, in this case the first trailing whitespace
		std::cout << "after triming key: " << key << "." << std::endl;
		value.erase(0, value.find_first_not_of(" \t"));
		value.erase(value.find_last_not_of(" \t") + 1);
		std::cout << "after triming value: " << value << "." << std::endl;
		std::cout << std::endl;

		// Check if key or value is empty
		if (key.empty())
		{
			std::cerr << "Empty date at line : " + line << std::endl;
			return ;
		}
		if (value.empty())
		{
			std::cerr << "Empty exchange rate at line : " + line << std::endl;
			return ;
		}

	}
}
