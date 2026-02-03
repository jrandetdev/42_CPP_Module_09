#include "PmergeMe.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <limits>
#include <exception>

void	checkNegativeNumber(int value)
{
	if (value < 0)
		throw std::runtime_error("Error: input cannot be a negative number");
}

void	checkIntMax(const std::string& input)
{
	if (input.length() > 11)
		throw std::runtime_error("Error: number is bigger than INTMAX.");
	else if (input.length() == 11)
	{
		const std::string intMax = "2147483647";
		for (size_t i = 0; i < input.length(); ++i)
		{
			if (input[i] < intMax[i])
				return ;
			if (input[i] > intMax[i])
				throw std::runtime_error("Error: number is bigger than INT_MAX 2147483647");
		}
	}
}

void	validatingArguments(int argc, char **argv, std::vector<int> &vecContainer)
{
	int value = 0;
	for (int i = 1; i < argc; ++i)
	{
		checkIntMax(argv[i]);
		std::stringstream ss(argv[i]);
		if (!(ss >> value))
			throw std::runtime_error("Error: program only accept positive integers.");
		checkNegativeNumber(value);
		vecContainer.push_back(value);
	}
}

int main(int argc, char **argv)
{
	if (argc <= 3)
	{
		std::cerr << RED << "Error: please input at least two positive intergers." << RESET << std::endl; // I think I will need more than one to even begin but for now this is it
		return 1;
	}
	std::vector<int> vecContainer;
	// std::deque<int> dequeContainer;
	try {
		validatingArguments(argc, argv, vecContainer);
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return 1;
	}

	std::cout << "all good !" << std::endl;
	// now I need to send my vector container for it to get sorted. 
	std::cout << vecContainer << std::endl;
	return 0;
}
