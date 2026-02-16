#include "PmergeMe.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <limits>
#include <exception>

bool	isSorted(std::deque<int> &result)
{
	std::deque<int>::iterator it;
	for (it = result.begin() + 1; it < result.end(); it++)
	{
		if (*(it) < *(it - 1))
			return false;
	}
	return true;
}

bool	isSorted(std::vector<int> &result)
{
	std::vector<int>::iterator it;
	for (it = result.begin() + 1; it < result.end(); it++)
	{
		if (*(it) < *(it - 1))
			return false;
	}
	return true;
}

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

void	validatingArguments(int argc, char **argv, std::vector<int> &vecContainer, std::deque<int> &initialElementsDeq)
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
		initialElementsDeq.push_back(value);
	}
}

int main(int argc, char **argv)
{
	if (argc <= 3)
	{
		std::cerr << RED << "Error: please input at least two positive intergers." << RESET << std::endl; // I think I will need more than one to even begin but for now this is it
		return 1;
	}
	std::vector<int> initialElementsVec;
	std::deque<int> initialElementsDeq;
	try {
		validatingArguments(argc, argv, initialElementsVec, initialElementsDeq);
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return 1;
	}
	
	std::vector<int> result = mergeInsert(initialElementsVec);
	if (!isSorted(result))
	{
		std::cout << "Vector Array is not sorted" << std::endl;
		return (1);
	}
	else
	{
		std::cout << "Vector Array is sorted!" << std::endl;
	}
	
	std::deque<int> resultD = mergeInsert(initialElementsDeq);
	if (!isSorted(resultD))
	{
		std::cout << "Deque Array is not sorted" << std::endl;
		return (1);
	}
	else
	{
		std::cout << "Deque Array is sorted!" << std::endl;
		return (1);
	}
	return 0;
}
