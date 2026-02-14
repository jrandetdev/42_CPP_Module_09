#include "PmergeMe.hpp"

#include <iterator>
#include <sstream>
#include <iostream>
#include <cmath>

unsigned int pairCompCounter = 0;
unsigned int insertCompCounter = 0;


//======================= PAIR CLASS METHODS =======================

Pair::Pair() : value(0), left(NULL), right(NULL) {}

Pair::Pair(int value) : value(value), left(NULL), right(NULL) {}

Pair::Pair(Pair *a, Pair *b) : value(b->value), left(a), right(b)
{
	if (!a || !b)
	return;
	pairCompCounter++;
	if (a->value > b->value)
	{
		this->value = a->value;
		this->left = b;
		this->right = a;
	}
}

Pair::~Pair() {}

//===================== MERGE INSERTION START ====================

void	updateUpperLimits(std::vector<size_t> &upperlimit, size_t insertionPoint)
{
	for (size_t i = 0; i < upperlimit.size(); ++i)
	{
		if (upperlimit[i] >= insertionPoint)
		upperlimit[i]++;
	}
}

size_t	insertElementInResult(std::vector<Pair *> &resultVector, Pair *elementToInsert, size_t upperLimit)
{
	std::vector<Pair *>::iterator low = resultVector.begin();	// iterators 
	std::vector<Pair *>::iterator high = resultVector.begin() + upperLimit;
	size_t difference;
	
	while (low < high)
	{
		difference = high - low;
		std::vector<Pair*>::iterator mid = low + difference / 2;
		if (elementToInsert->value >= (*mid)->value)
		{
			low = mid + 1;
			insertCompCounter++;
		}
		else
		{
			high = mid;
			insertCompCounter++;
		}
	}
	resultVector.insert(low, elementToInsert);
	return (low - resultVector.begin());
}

void	buildUpperLimitArray(std::vector<size_t> &upperLimit, const std::vector<Pair *> &smaller, bool freeElementInserted)
{
	size_t i;
	freeElementInserted ? i = 2 : i = 1;
	for (size_t j = 0; j < smaller.size(); ++j)
	{
		upperLimit.push_back(i);
		++i;
	}
}

void	buildSmallerVector(const std::vector<Pair *> &pairs, std::vector<Pair *> &smaller)
{
	for (size_t i = 1; i < pairs.size(); ++i)
		smaller.push_back(pairs[i]->left);
}

void	buildResultVector(const std::vector<Pair *> &pairs, std::vector<Pair *> &result, bool &freeElementInserted)
{
	// BUILDING THE RESULT VECTOR
	for (size_t i = 0; i < pairs.size(); ++i)
		result.push_back(pairs[i]->right);
	
	if (pairs[0]->left)
	{
		result.insert(result.begin(), pairs[0]->left);
		freeElementInserted = true;
	}
}


std::vector<Pair *>	sortTree(std::vector<Pair *> &pairs)
{
	if (pairs[0]->right == NULL)
		return pairs;

	std::vector<Pair *> result;
	std::vector<Pair *> smaller;
	std::vector<size_t> upperLimit;
	bool freeElementInserted = false;

	buildResultVector(pairs, result, freeElementInserted);
	buildSmallerVector(pairs, smaller);
	buildUpperLimitArray(upperLimit, smaller, freeElementInserted);

	std::cout << "\nsmaller elements to be inserted in result: " << smaller << std::endl;
	std::cout << "upperlimit array: " << upperLimit << std::endl;
	
	size_t insertionPoint;
	for (size_t i = 0; i < smaller.size(); ++i)
	{
		if (smaller[i])
		{
			insertionPoint = insertElementInResult(result, smaller[i], upperLimit[i]);
			updateUpperLimits(upperLimit, insertionPoint);
		}
	}
	
	std::cout << "\nafter insert sort, result: " << result << std::endl;
	return (sortTree(result));
}

std::vector<Pair *> groupIntoPairs(std::vector<Pair *> pairs)
{
	std::vector<Pair *> treeFloor;
	std::vector<Pair *>::iterator it;
	std::vector<Pair *>::iterator lastElement;
	
	if (pairs.size() == 1)
	{
		std::cout << GREEN << "\nthe top of my recursion tree" << RESET << std::endl;
		printTree(pairs[0]);
		return (pairs);
	}

	if ((pairs.size() % 2) == 0)
		lastElement = pairs.end();
	else
		lastElement = pairs.end() - 1;
	
	for (it = pairs.begin(); it != lastElement; it += 2)
		treeFloor.push_back(new Pair(*it, *(it + 1)));
	
	if (lastElement != pairs.end())
		treeFloor.push_back(new Pair(NULL, *lastElement));
	
	return (groupIntoPairs(treeFloor));
}

std::vector<int> pairsToInt(const std::vector<Pair *> pairs)
{
	std::vector<int> intResult;
	std::vector<Pair *>::const_iterator it;
	for (it = pairs.begin(); it < pairs.end(); ++it)
	intResult.push_back((*it)->value);
	return (intResult);
}

void	intToPairs(const std::vector<int> &initialElementsVec, std::vector<Pair *> &pairs)
{
	for (size_t i = 0; i < initialElementsVec.size(); ++i)
	pairs.push_back(new Pair(initialElementsVec[i]));
}

int idealComparisonNumber(int n)
{
	int sum = 0;
	for (int k = 1; k <= n; ++k) {
		double value = (3.0 / 4.0) * k;
		sum += static_cast<int>(std::ceil(std::log2(value)));
	}
	return sum;
}

std::vector<int> mergeInsert(std::vector<int> &initialElementsVec)
{
	std::vector<Pair *> pairs;
	std::vector<Pair *> dummy;
	std::vector<Pair *> result;
	
	intToPairs(initialElementsVec, pairs);
	dummy = groupIntoPairs(pairs);
	result = sortTree(dummy);
	std::cout << "Comparisons made during pair making: " << pairCompCounter 
	<< " and comparisons made during insertion " << insertCompCounter
	<< " and total: " << pairCompCounter + insertCompCounter << std::endl;
	
	std::cout << "Ideal number of comparisons for " << initialElementsVec.size()
	<< " is " << idealComparisonNumber(initialElementsVec.size())
	<< std::endl;
	return (pairsToInt(result));
}

// void	_deleteTree(Pair* node)
// {
// 	if (node == NULL) return;
// 	_deleteTree(node->left);
// 	_deleteTree(node->right);
// 	delete node;
// }

//function to delete the tree
// void	deleteTree(Pair **nodeRef)
// {
// 	_deleteTree(*nodeRef);
// 	*nodeRef = NULL;
// }
//==================== DEBUG OUTPUT TO SEE PAIR ====================

std::ostream &operator<<(std::ostream& outstream, std::vector<Pair *> pairs)
{
	std::vector<Pair *>::iterator it;

	//outstream << '\n';
	for (it = pairs.begin(); it < pairs.end(); ++it)
	{
		if (!(*it))
		{
			std::cout << " NULL ";
			continue;
		}

		if ((*it)->left)
			outstream << '[' << MAG << (*it)->left->value << RESET; // Left part
		else
			outstream << '[' << MAG << "X" << RESET;

		outstream << " " << (*it)->value << " ";
		if ((*it)->right)
			outstream << MAG << (*it)->right->value << RESET << ']';
		else
			outstream << MAG << "X" << RESET << ']';

		if (it < pairs.end() - 1)
			outstream << " ";
	}
	return outstream;
}


void printTree(Pair* root, std::string indent, bool isLeft)
{
	if (root == NULL)
	{
		std::cout << indent << (isLeft ? "└── " : "┌── ") << RED << "NULL" << RESET << std::endl;
		return;
	}

	if (root->right || root->left)
	{
		if (root->right)
			printTree(root->right, indent + (isLeft ? "│   " : "    "), false);
		else
			std::cout << indent << (isLeft ? "│   " : "    ") << "┌── " << RED << "X" << RESET << std::endl;
	}

	std::cout << indent;
	if (isLeft)
		std::cout << "└── ";
	else
		std::cout << "┌── ";
	
	std::cout << "[" << root->value << "]" << std::endl;

	if (root->left || root->right)
	{
		if (root->left)
			printTree(root->left, indent + (isLeft ? "    " : "│   "), true);
		else
			std::cout << indent << (isLeft ? "    " : "│   ") << "└── " << RED << "X" << RESET << std::endl;
	}
}
