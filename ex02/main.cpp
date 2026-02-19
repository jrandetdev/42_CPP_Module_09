#include "PmergeMe.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <limits>
#include <exception>

static timeval getTime()
{
	timeval now;
	gettimeofday(&now, NULL);
	return now;
}

static double usElapsedFrom(timeval start)
{
	timeval now;

	gettimeofday(&now, NULL);
	double seconds = static_cast<double>(now.tv_sec - start.tv_sec);
	double microseconds = static_cast<double>(now.tv_usec - start.tv_usec);
	return seconds * 1000000.0 + microseconds;
}

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
		std::cerr << RED << "Error: please input at least two positive integers." << RESET << std::endl; // I think I will need more than one to even begin but for now this is it
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
	timeval start = getTime();
	std::vector<int> result = mergeInsert(initialElementsVec);
	double elapsed_time = usElapsedFrom(start);
	if (!isSorted(result))
	{
		std::cerr << "Vector Array is not sorted" << std::endl;
		return (1);
	}
	else
	{
		std::cout << "Before:	" << initialElementsVec << std::endl;
		std::cout << "After:	" << result << std::endl;
		std::cout << "Time to process a range of " << initialElementsVec.size()\
					<< " elements with std::vector: : " << std::fixed << std::setprecision(5) <<  elapsed_time << " us" << std::endl;
	}
	
	start = getTime();
	std::deque<int> resultD = mergeInsert(initialElementsDeq);
	elapsed_time = usElapsedFrom(start);
	if (!isSorted(resultD))
	{
		std::cerr << "Deque Array is not sorted" << std::endl;
		return (1);
	}
	else
	{
		std::cout << "Time to process a range of " << initialElementsDeq.size()\
					<< " elements with std::deque: : " << elapsed_time << " us" << std::endl;
	}
	return 0;
}
