#include "PmergeMe.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <limits>
#include <exception>



void	validatingArguments(int argc, char **argv, std::vector<int> &vecContainer)
{
	int value;
	for (int i = 1; i < argc; ++i)
	{
		std::stringstream ss(argv[i]);
		if (!(ss >> value))
			throw std::runtime_error("Error: argument " + std::string(argv[i]) + " is not a positive integer.");
		
		if (value < 0)
			throw std::runtime_error("Error: argument cannot be negative");
		
		if ( INT_MAX > INT_MAX )
		
		vecContainer.push_back(value);
	}
}

int main(int argc, char **argv)
{
	if (argc <= 3)
	{
		std::cerr << "Error: please input at least two positive intergers." << std::endl; // I think I will need more than one to even begin but for now this is it
		return 1;
	}
	std::vector<int> vecContainer;
	// std::deque<int> dequeContainer;

	try 
	{
		validatingArguments(argc, argv, vecContainer);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::cout << "all good !" << std::endl;
	return 0;
}
