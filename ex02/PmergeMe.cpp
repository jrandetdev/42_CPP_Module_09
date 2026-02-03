#include "PmergeMe.hpp"

std::ostream &operator<<(std::ostream& ostream, std::vector<int> vectorContainer)
{
	ostream << "Before:	";
	for (size_t i = 0; i < vectorContainer.size(); ++i)
	{
		ostream << vectorContainer[i];
		if (i < vectorContainer.size() - 1)
			ostream << " ";
	}

	return ostream;
}
