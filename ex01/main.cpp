#include "RPN.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>

int main(int argc, char **argv)
{
	if (argc != 2) 
	{
		std::cout << "Error: program takes one and only one mathematical expression in reverse polish notation." << std::endl;
		return (1);
	}

	
	std::string e = argv[1];
	std::stringstream ss(e);
	std::string token;

	while (ss >> token)
	{
		
	}
 
	return (0);
}

std::string 