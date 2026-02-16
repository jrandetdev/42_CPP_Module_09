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

// ===================== JACOB STYLE ============================

int	getMinIndex(int a, int b)
{
	return (a < b ? a : b);
}

void generateJacobIndexes(size_t size, std::vector<int> &jacobIndexes) {
	// std::vector<int> indexes(size);
	const int jacobSequence[] = {1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203};
	int jacobIndex = 1;
	size_t index = 0;
	// taille des groupes c'est number - previous number
	while (index < size)
	{
		int number = getMinIndex(jacobSequence[jacobIndex] - 2, static_cast<int>(size - 1));
		int previousNumber = jacobSequence[jacobIndex - 1] - 2;
		while (number > previousNumber)
		{
			jacobIndexes[number] = index; // 0
			index++; // 1
			number--;
		}
		jacobIndex++;
	}
}

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
	size_t insertionPoint;
	
	while (low < high)
	{
		difference = high - low;
		std::vector<Pair*>::iterator mid = low + difference / 2;
		if (elementToInsert->value >= (*mid)->value)
		{
			low = mid + 1;
			insertCompCounter
		++;
		}
		else
		{
			high = mid;
			insertCompCounter
		++;
		}
	}
	insertionPoint = low - resultVector.begin();	
	resultVector.insert(low, elementToInsert);
	return (insertionPoint);
}

void	buildUpperLimitArray(std::vector<size_t> &upperLimit, const std::vector<Pair *> &smaller, bool freeElementInserted)
{
	size_t i = freeElementInserted ? 2 : 1;
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

std::vector<Pair *>	sortAndInsertByJacobStahl(std::vector<Pair *> &pairs)
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

	std::cout << "\nresult array: " << result << std::endl;
	std::cout << "smaller elements to be inserted in result: " << smaller << std::endl;
	std::cout << "upperlimit array: " << upperLimit << std::endl;
	
	std::vector<int> jacobIndexes(smaller.size());
	generateJacobIndexes(smaller.size(), jacobIndexes);
	size_t insertionPoint;
	for (size_t i = 0; i < smaller.size(); ++i)
	{
		int index = jacobIndexes[i];
		// int index = i;
		if (smaller[index])
		{
			insertionPoint = insertElementInResult(result, smaller[index], upperLimit[index]);
			updateUpperLimits(upperLimit, insertionPoint);
			std::cout << "inserted the number " << smaller[index]->value << std::endl;
			std::cout << "result array: " << result << std::endl;
			std::cout << "upperlimit array: " << upperLimit << std::endl;
		}
	}
	
	std::cout << "\nafter insert sort, result: " << result << std::endl;
	return (sortAndInsertByJacobStahl(result));
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
	pairCompCounter = 0;
	insertCompCounter = 0;
	std::vector<Pair *> pairs;
	std::vector<Pair *> dummy;
	std::vector<Pair *> result;
	
	intToPairs(initialElementsVec, pairs);
	dummy = groupIntoPairs(pairs);
	result = sortAndInsertByJacobStahl(dummy);

	std::cout << "Comparisons made during pair making: " << pairCompCounter 
	<< " and comparisons made during insertion " << insertCompCounter

	<< " and total: " << pairCompCounter + insertCompCounter
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

void generateJacobIndexes(size_t size, std::deque<int> &jacobIndexes) {
	// std::deque<int> indexes(size);
	const int jacobSequence[] = {1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203};
	int jacobIndex = 1;
	size_t index = 0;
	// taille des groupes c'est number - previous number
	while (index < size)
	{
		int number = getMinIndex(jacobSequence[jacobIndex] - 2, static_cast<int>(size - 1));
		int previousNumber = jacobSequence[jacobIndex - 1] - 2;
		while (number > previousNumber)
		{
			jacobIndexes[number] = index; // 0
			index++; // 1
			number--;
		}
		jacobIndex++;
	}
}

//=====================  VECTOR MERGE INSERTION START ====================

void	updateUpperBound(std::deque<size_t> &upperlimit, size_t insertionPoint)
{
	for (size_t i = 0; i < upperlimit.size(); ++i)
	{
		if (upperlimit[i] >= insertionPoint)
			upperlimit[i]++;
	}
}

size_t	insertPendingElement(std::deque<Pair *> &resultVector, Pair *elementToInsert, size_t upperLimit)
{
	std::deque<Pair *>::iterator low = resultVector.begin();	// iterators 
	std::deque<Pair *>::iterator high = resultVector.begin() + upperLimit;
	size_t difference;
	size_t insertionPoint;
	
	while (low < high)
	{
		difference = high - low;
		std::deque<Pair*>::iterator mid = low + difference / 2;
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
	insertionPoint = low - resultVector.begin();	
	resultVector.insert(low, elementToInsert);
	return (insertionPoint);
}

void	updateUpperBound(std::deque<size_t> &upperLimit, const std::deque<Pair *> &smaller, bool freeElementInserted)
{
	size_t i = freeElementInserted ? 2 : 1;
	for (size_t j = 0; j < smaller.size(); ++j)
	{
		upperLimit.push_back(i);
		++i;
	}
}

void	buildPending(const std::deque<Pair *> &pairs, std::deque<Pair *> &smaller)
{
	for (size_t i = 1; i < pairs.size(); ++i)
		smaller.push_back(pairs[i]->left);
}

void	buildMainChain(const std::deque<Pair *> &pairs, std::deque<Pair *> &result, bool &freeElementInserted)
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

std::deque<Pair *>	sortAndInsertByJacobStahl(std::deque<Pair *> &pairs)
{
	if (pairs[0]->right == NULL)
		return pairs;

	std::deque<Pair *> result;
	std::deque<Pair *> smaller;
	std::deque<size_t> upperLimit;
	bool freeElementInserted = false;

	buildMainChain(pairs, result, freeElementInserted);
	buildPending(pairs, smaller);
	updateUpperBound(upperLimit, smaller, freeElementInserted);
	
	std::deque<int> jacobIndexes(smaller.size());
	generateJacobIndexes(smaller.size(), jacobIndexes);
	size_t insertionPoint;
	for (size_t i = 0; i < smaller.size(); ++i)
	{
		int index = jacobIndexes[i];
		// int index = i;
		if (smaller[index])
		{
			insertionPoint = insertPendingElement(result, smaller[index], upperLimit[index]);
			updateUpperBound(upperLimit, insertionPoint);
		}
	}
	return (sortAndInsertByJacobStahl(result));
}

std::deque<Pair *> buildSortedPairTree(std::deque<Pair *> pairs)
{
	std::deque<Pair *> treeFloor;
	std::deque<Pair *>::iterator it;
	std::deque<Pair *>::iterator lastElement;
	
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
	
	return (buildSortedPairTree(treeFloor));
}

std::deque<int> pairsToInt(const std::deque<Pair *> pairs)
{
	std::deque<int> intResult;
	std::deque<Pair *>::const_iterator it;
	for (it = pairs.begin(); it < pairs.end(); ++it)
		intResult.push_back((*it)->value);
	return (intResult);
}

void	intToPairs(const std::deque<int> &initialElements, std::deque<Pair *> &pairs)
{
	for (size_t i = 0; i < initialElements.size(); ++i)
		pairs.push_back(new Pair(initialElements[i]));
}

std::deque<int> mergeInsert(std::deque<int> &initialElements)
{
	pairCompCounter = 0;
	insertCompCounter = 0;

	std::deque<Pair *> pairs;
	std::deque<Pair *> dummy;
	std::deque<Pair *> result;
	
	intToPairs(initialElements, pairs);
	dummy = buildSortedPairTree(pairs);
	result = sortAndInsertByJacobStahl(dummy);
	std::cout << "Comparisons made during pair making: " << pairCompCounter 
	<< " and comparisons made during insertion " << insertCompCounter
	<< " and total: " << pairCompCounter + insertCompCounter << std::endl;
	
	std::cout << "Ideal number of comparisons for " << initialElements.size()
	<< " is " << idealComparisonNumber(initialElements.size())
	<< std::endl;
	return (pairsToInt(result));
}
