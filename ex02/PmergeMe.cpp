#include "PmergeMe.hpp"

#include <iterator>

//====================== OUTPUT OPERATORS ========================

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

//==================== DEBUG OUTPUT TO SEE PAIR ====================

std::ostream &operator<<(std::ostream& ostream, std::vector<Pair> pairs)
{

	std::vector<Pair>::iterator it;

	for (it = pairs.begin(); it < pairs.end(); ++it)
	{
		std::string valStr = std::to_string(it->value);
		int totalWidth = 11;
		int padding = (totalWidth - valStr.length()) / 2;

		ostream << '|' << std::setw(2) << it->left; // Left part

		ostream << std::string(padding, ' ') << valStr 
				<< std::string(totalWidth - padding - valStr.length(), ' ');

		ostream << std::setw(2) << it->right << '|';

		if (it < pairs.end() - 1)
			ostream << "	";
	}
	return ostream;
}

//======================= PAIR CLASS METHODS =======================

Pair::Pair() : value(0), left(nullptr), right(nullptr) {}

// for the base scenario where we construct only the leaves of the tree 
Pair::Pair(int value) : value(value), left(nullptr), right(nullptr) {}

// for the recustive method where we need to create a pair made of two pairs
Pair::Pair(Pair *a, Pair *b)
{
	// by defaulét do something, and else swap could be something I could do
	if (a->value > b->value)
	{
		this->value = a->value;
		this->left = b;
		this->right = a;
	}
	else
	{
		this->value = b->value;
		this->left = a;
		this->right = b;
	}
}

Pair::~Pair()
{
	delete this->left;
	delete this->right;
}

//===================== MERGE INSERTION START ====================

void	mergeInsert(std::vector<int> &initialElementsVec)
{
	std::vector<Pair> pairs;
	groupElementsIntoPairs(initialElementsVec, pairs);
	std::cout << pairs << std::endl;

}

void	groupElementsIntoPairs(const std::vector<int> &initialElementsVec, std::vector<Pair> &pairs)
{
	// std::vector<int>::iterator initialElementsIt;
	for (size_t i = 0; i < initialElementsVec.size(); ++i)
	{
		pairs.push_back(Pair(initialElementsVec[i]));
	}
}
