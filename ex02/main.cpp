#include "PmergeMe.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>

bool	parseArguments(int argc, char **argv, std::vector<int> &vecContainer)
{
	int value;
	for (int i = 1; i < argc; ++i)
	{
		std::stringstream ss(argv[i]);
		if (!(ss >> value) || value < 0)
		{
			std::cout << "Error: argument " << argv[i] << " is not a positive integer." << std::endl;
			return false;
		}
		vecContainer.push_back(value);
		// dequeContainer.push_back(value);
	}
	return true;
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

	if (!parseArguments(argc, argv, vecContainer))
		return 1;
	std::cout << "all good !" << std::endl;
	return 0;
}
