#include "RPN.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>

bool	isValidExpression(const std::string& expression);

int main(int argc, char **argv)
{
	if (argc < 2) 
	{
		std::cout << "Error: program takes at least one and only one "
		"mathematical expression in reverse polish notation." << std::endl;
		return (1);
	}
	else if (argc > 2)
	{
		std::cout << "Error: program takes not more than one "
		"mathematical expression in reverse polish notation." << std::endl;
		return (1);
	}

	try {
		RPN rpn(argv[1]);
	}
	catch (std::exception &e )
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	return (0);
}

