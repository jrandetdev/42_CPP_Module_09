/**
 * @file PmergeMe.cpp
 * @author Josephine Randet
 * @brief Implementation of the Ford-Johnson algorithm for positive integer sorting
 * @date 2026-02-19
 */

#include "PmergeMe.hpp"

#include <iterator>
#include <sstream>
#include <iostream>
#include <cmath>

unsigned int pairCompCounter = 0;
unsigned int insertCompCounter = 0;

// Main logic of mergeInsert
static void					intToPairs(const std::vector<int> &initialElementsVec, std::vector<Pair *> &pairs);
static std::vector<Pair *>	buildSortedPairTree(std::vector<Pair *> pairs);
static std::vector<Pair *>	sortAndInsertByJacobStahl(std::vector<Pair *> &pairs);
static std::vector<int> 	pairsToInt(const std::vector<Pair *> pairs);
static void					_deleteTree(Pair *node);
static void					deleteTree(Pair **nodeRef);
// static int					idealComparisonNumber(int numberOfElements);

// Main logic of SortAndInsertByJacobStahl
static void					buildMainChain(const std::vector<Pair *> &pairs, std::vector<Pair *> &result, bool &freeElementInserted);
static void					buildPending(const std::vector<Pair *> &pairs, std::vector<Pair *> &smaller);
static void					buildUpperLimitArray(std::vector<size_t> &upperLimit, const std::vector<Pair *> &smaller, bool freeElementInserted);
static void					generatejacobstahlLookupOrder(size_t numberOfPendingElements, std::vector<int> &jacobstahlLookupOrder);
static int					getMinIndex(int a, int b);
static size_t				binaryInsertElement(std::vector<Pair *> &resultVector, Pair *elementToInsert, size_t upperLimit);
static void					updateUpperBoundAfterInsertion(std::vector<size_t> &upperlimit, size_t insertionPoint);

// =============================================================================
// MERGE INSERTION LOGIC FOR STD::VECTOR
// =============================================================================


std::vector<int> mergeInsert(std::vector<int> &initialElementsVec)
{
	pairCompCounter = 0;
	insertCompCounter = 0;
	
	std::vector<Pair *> pairs;
	std::vector<Pair *> finalPair;
	std::vector<Pair *> sortedElements;
	std::vector<int> finalResult;
	
	intToPairs(initialElementsVec, pairs);		// Transform the initial list into the Pair format (Pair being the class I have)
	finalPair = buildSortedPairTree(pairs);				// 
	sortedElements = sortAndInsertByJacobStahl(finalPair);
	finalResult = pairsToInt(sortedElements);
	
	DEBUG(std::cout << "Comparisons made during pair making: " << pairCompCounter)
	DEBUG(<< " and comparisons made during insertion " << insertCompCounter)
	DEBUG(<< " and total: " << pairCompCounter + insertCompCounter)
	DEBUG(<< std::endl;)
	DEBUG(std::cout << "Ideal number of comparisons for " << initialElementsVec.size()
	<< " is " << idealComparisonNumber(initialElementsVec.size())
	<< std::endl;)
	deleteTree(&finalPair[0]);
	return (finalResult);
}

//	Utility for seeing if implementation meets requirements 
// int idealComparisonNumber(int numberOfElements)
// {
// 	int sum = 0;
// 	for (int k = 1; k <= numberOfElements; ++k) {
// 		double value = (3.0 / 4.0) * k;
// 		sum += static_cast<int>(std::ceil(log2(value)));
// 	}
// 	return sum;
// }

// ==============================================================================================================================
// RECURSIVE LOGIC FOR FIRST HALF OF FORD-JOHNSON (BUILDING TREE OF SORTED PAIRS + SORTING N/2 PAIRS ACCORDING TO LARGEST ELEMENT
// ==============================================================================================================================

static std::vector<Pair *> buildSortedPairTree(std::vector<Pair *> pairs)
{
	std::vector<Pair *> treeFloor;
	std::vector<Pair *>::iterator it;
	std::vector<Pair *>::iterator lastElement;
	
	if (pairs.size() == 1)
	{
		DEBUG(std::cout << GREEN << "\nthe top of my recursion tree" << RESET << std::endl;)
		DEBUG(printTree(pairs[0]);)
		return (pairs);
	}

	if ((pairs.size() % 2) == 0)
		lastElement = pairs.end();
	else
		lastElement = pairs.end() - 1;
	
	for (it = pairs.begin(); it != lastElement; it += 2)
		treeFloor.push_back(new Pair(*it, *(it + 1)));

	if (lastElement != pairs.end())
		treeFloor.push_back(new Pair(NULL, *lastElement)); // special edge case for ODD element, the pair has a NULL child on the left

	return (buildSortedPairTree(treeFloor));
}

// CONSTRUCTORS AND METHODS USED TO ORGANISE THE DATA (REFER TO HPP FILE FOR MORE INFO)

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

// ====================================================================================================================================================
// RECURSIVE LOGIC FOR SECOND HALF OF FORD-JOHNSON ALRORITHM: INSERTING SMALLER B INTO MAIN CHAIN A USING BINARY INSERTION IN THE JACOB SEQUENCE ORDER
// ====================================================================================================================================================

static std::vector<Pair *>	sortAndInsertByJacobStahl(std::vector<Pair *> &pairs)
{
	if (pairs[0]->right == NULL)
		return pairs;

	std::vector<Pair *> mainChain;
	std::vector<Pair *> pending;
	std::vector<size_t> upperLimit;
	bool freeElementInserted = false;

	buildMainChain(pairs, mainChain, freeElementInserted);
	buildPending(pairs, pending);
	buildUpperLimitArray(upperLimit, pending, freeElementInserted);

	DEBUG(std::cout << "\nmainChain array: " << mainChain << std::endl;)
	DEBUG(std::cout << "pending elements to be inserted in mainChain: " << pending << std::endl;)
	DEBUG(std::cout << "upperlimit array: " << upperLimit << std::endl;)
	
	std::vector<int> jacobstahlLookupOrder(pending.size());
	generatejacobstahlLookupOrder(pending.size(), jacobstahlLookupOrder);
	size_t insertionPoint;

	for (size_t i = 0; i < pending.size(); ++i)
	{
		int index = jacobstahlLookupOrder[i];
		if (pending[index])
		{
			insertionPoint = binaryInsertElement(mainChain, pending[index], upperLimit[index]);
			updateUpperBoundAfterInsertion(upperLimit, insertionPoint);
			DEBUG(std::cout << "inserted the number " << pending[index]->value << std::endl;)
			DEBUG(std::cout << "upperlimit array: " << upperLimit << std::endl;)
			DEBUG(std::cout << "mainChain array: " << mainChain << std::endl;)
		}
	}
	
	DEBUG(std::cout << "\nafter insert sort, mainChain: " << mainChain << std::endl;)
	return (sortAndInsertByJacobStahl(mainChain));
}

/**
 * @brief Generates the lookup table where each index in the pending vector is assigned a priority
 * according to the jacobSequence
 * 
 * 1. Move to the next JacobStahl number (jacobIndex)
 * 
 * 2. Process elements from jacobSequence[n] down to jacobsequence[n - 1]. By inserting from the
 * top of each block downwards, we ensure the binary search range never exceeds 2k−1 prematurely,
 * minimizing the total number of comparisons to near the information-theoretic lower bound.
 * We update the upper limit each time to make sure that the binary search is always performed
 * with highest = the most efficient value. This is more efficient than a classic binary search.
 * 
 * @param numberOfPendingElements The number of pending elements to be mapped in the lookup table
 * @param jacobstahlLookupOrder A vector where jacobstahlLookupOrder are stored in the execution
 * order for the element at index i
 */
static void generatejacobstahlLookupOrder(size_t numberOfPendingElements, std::vector<int> &jacobstahlLookupOrder)
{
	// special seuquence where it is always one away from 2k += 1
	const int jacobSequence[] = {1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203};
	
	int jacobIndex = 1;	// JacobIndex corresponds to the index we are in in the sequence. We begin at jacobSequence[1] = 3.
	size_t lookUpTableIndex = 0; // the position index in our 
	
	
	//std::cout << "\nsize received: " << numberOfPendingElements << std::endl;

	while (lookUpTableIndex < numberOfPendingElements)	// going through the jacobsequence as long as index is smaller than numberOfPendingElements
	{
		// Define the boundaries of the current block according to the jacobSequence, and numberOfPendingElements if the numberOfPendingElements > jacobSequence[jacobIndex]
		int number = getMinIndex(jacobSequence[jacobIndex] - 2, static_cast<int>(numberOfPendingElements - 1));
		int previousNumber = jacobSequence[jacobIndex - 1] - 2;

		// Fill the current lookup table then used by sortAndInsertByJacobStrahl in reverse order
		while (number > previousNumber)	// 
		{
			jacobstahlLookupOrder[number] = lookUpTableIndex;
			lookUpTableIndex++;
			number--;
		}
		jacobIndex++;
	}
}

// Helper function in case where size is bigger than the jacobstahl index, but smaller than the next one.
static int	getMinIndex(int a, int b)
{
	return (a < b ? a : b);
}

// ===========================================================================================================================================================
// INSERTING FUNCTIONS THAT MANIPULATE THE CONTAINERS FOR THE MAIN CHAIN, PENDING CHAIN, AND UPPERLIMIT INDEX ARRAY USED TO OPTIMISE THE AMOUNT OF COMPARISONS
// ===========================================================================================================================================================

static void	buildMainChain(const std::vector<Pair *> &pairs, std::vector<Pair *> &mainChain, bool &freeElementInserted)
{
	for (size_t i = 0; i < pairs.size(); ++i)
		mainChain.push_back(pairs[i]->right);
	
	if (pairs[0]->left)
	{
		mainChain.insert(mainChain.begin(), pairs[0]->left);
		freeElementInserted = true;
	}
}

static void	buildPending(const std::vector<Pair *> &pairs, std::vector<Pair *> &pending)
{
	for (size_t i = 1; i < pairs.size(); ++i)
		pending.push_back(pairs[i]->left);
}

static void	buildUpperLimitArray(std::vector<size_t> &upperLimit, const std::vector<Pair *> &pending, bool freeElementInserted)
{
	size_t i = freeElementInserted ? 2 : 1;
	for (size_t j = 0; j < pending.size(); ++j)
	{
		upperLimit.push_back(i);
		++i;
	}
}

static size_t	binaryInsertElement(std::vector<Pair *> &mainChainVector, Pair *elementToInsert, size_t upperLimit)
{
	std::vector<Pair *>::iterator low = mainChainVector.begin();	// iterators 
	std::vector<Pair *>::iterator high = mainChainVector.begin() + upperLimit;
	size_t difference;
	size_t insertionPoint;
	
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
			high = mid; // the reason for the +1 not here is because of the exclusion [ ( of the insert function.
			insertCompCounter++;
		}
	}
	insertionPoint = low - mainChainVector.begin();	
	mainChainVector.insert(low, elementToInsert);
	return (insertionPoint);
}

/**
 * @brief Updates the upper bound indices for pending elements to match the one in the result indices after insertion.
 * 
 * @param upperlimit 
 * @param insertionPoint 
 */
static void	updateUpperBoundAfterInsertion(std::vector<size_t> &upperlimit, size_t insertionPoint)
{
	for (size_t i = 0; i < upperlimit.size(); ++i)
	{
		if (upperlimit[i] >= insertionPoint)
			upperlimit[i]++;
	}
}

// =============================================================================================
// DATA MAPPING FROM INT TO PAIRS AND PAIRS TO INT AND MEMORY MANAGEMENT
// =============================================================================================

static void	intToPairs(const std::vector<int> &initialElementsVec, std::vector<Pair *> &pairs)
{
	for (size_t i = 0; i < initialElementsVec.size(); ++i)
		pairs.push_back(new Pair(initialElementsVec[i]));
}

static std::vector<int> pairsToInt(const std::vector<Pair *> pairs)
{
	std::vector<int> intResult;
	std::vector<Pair *>::const_iterator it;
	for (it = pairs.begin(); it < pairs.end(); ++it)
		intResult.push_back((*it)->value);
	return (intResult);
}

static void	_deleteTree(Pair *node)
{
	if (node == NULL) return;
	_deleteTree(node->left);
	_deleteTree(node->right);
	delete node;
}

static void	deleteTree(Pair **nodeRef)
{
	_deleteTree(*nodeRef);
	*nodeRef = NULL;
}

// END STD::VECTOR







// FORWARD DECLARATIONS DEQUE

static void	intToPairs(const std::deque<int> &initialElements, std::deque<Pair *> &pairs);
static std::deque<Pair *> buildSortedPairTree(std::deque<Pair *> pairs);
static std::deque<Pair *>	sortAndInsertByJacobStahl(std::deque<Pair *> &pairs);
static std::deque<int> pairsToInt(const std::deque<Pair *> pairs);

static void	buildMainChain(const std::deque<Pair *> &pairs, std::deque<Pair *> &result, bool &freeElementInserted);
static void	buildPending(const std::deque<Pair *> &pairs, std::deque<Pair *> &smaller);
static void	buildUpperLimitArray(std::deque<size_t> &upperLimit, const std::deque<Pair *> &smaller, bool freeElementInserted);
static void generateJacobIndexes(size_t size, std::deque<int> &jacobIndexes);
static size_t	binaryInsertElement(std::deque<Pair *> &resultVector, Pair *elementToInsert, size_t upperLimit);
static void	updateUpperBoundAfterInsertion(std::deque<size_t> &upperlimit, size_t insertionPoint);


// =============================================================================
// MERGE INSERTION LOGIC FOR STD::DEQUE
// =============================================================================

std::deque<int> mergeInsert(std::deque<int> &initialElements)
{
	pairCompCounter = 0;
	insertCompCounter = 0;

	std::deque<Pair *> pairs;
	std::deque<Pair *> root;
	std::deque<Pair *> result;
	std::deque<int>	finalResult;
	
	intToPairs(initialElements, pairs);
	root = buildSortedPairTree(pairs);
	result = sortAndInsertByJacobStahl(root);
	finalResult = pairsToInt(result);
	DEBUG(std::cout << "Comparisons made during pair making: " << pairCompCounter 
	<< " and comparisons made during insertion " << insertCompCounter
	<< " and total: " << pairCompCounter + insertCompCounter << std::endl);
	
	DEBUG(std::cout << "Ideal number of comparisons for " << initialElements.size()
	<< " is " << idealComparisonNumber(initialElements.size())
	<< std::endl;)
	deleteTree(&root[0]);
	return (finalResult);
}

// ==============================================================================================================================
// RECURSIVE LOGIC FOR FIRST HALF OF FORD-JOHNSON (BUILDING TREE OF SORTED PAIRS + SORTING N/2 PAIRS ACCORDING TO LARGEST ELEMENT
// ==============================================================================================================================

static std::deque<Pair *> buildSortedPairTree(std::deque<Pair *> pairs)
{
	std::deque<Pair *> treeFloor;
	std::deque<Pair *>::iterator it;
	std::deque<Pair *>::iterator lastElement;
	
	if (pairs.size() == 1)
	{
		DEBUG(std::cout << GREEN << "\nthe top of my recursion tree" << RESET << std::endl;)
		DEBUG(printTree(pairs[0]);)
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

// ====================================================================================================================================================
// RECURSIVE LOGIC FOR SECOND HALF OF FORD-JOHNSON ALRORITHM: INSERTING SMALLER B INTO MAIN CHAIN A USING BINARY INSERTION IN THE JACOB SEQUENCE ORDER
// ====================================================================================================================================================

static std::deque<Pair *>	sortAndInsertByJacobStahl(std::deque<Pair *> &pairs)
{
	if (pairs[0]->right == NULL)
		return pairs;

	std::deque<Pair *> result;
	std::deque<Pair *> smaller;
	std::deque<size_t> upperLimit;
	bool freeElementInserted = false;

	buildMainChain(pairs, result, freeElementInserted);
	buildPending(pairs, smaller);
	buildUpperLimitArray(upperLimit, smaller, freeElementInserted);
	
	std::deque<int> jacobIndexes(smaller.size());
	generateJacobIndexes(smaller.size(), jacobIndexes);
	size_t insertionPoint;
	for (size_t i = 0; i < smaller.size(); ++i)
	{
		int index = jacobIndexes[i];
		// int index = i;
		if (smaller[index])
		{
			insertionPoint = binaryInsertElement(result, smaller[index], upperLimit[index]);
			updateUpperBoundAfterInsertion(upperLimit, insertionPoint);
		}
	}
	return (sortAndInsertByJacobStahl(result));
}

static void generateJacobIndexes(size_t size, std::deque<int> &jacobIndexes) {
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

// ===========================================================================================================================================================
// INSERTING FUNCTIONS THAT MANIPULATE THE CONTAINERS FOR THE MAIN CHAIN, PENDING CHAIN, AND UPPERLIMIT INDEX ARRAY USED TO OPTIMISE THE AMOUNT OF COMPARISONS
// ===========================================================================================================================================================

static void	buildMainChain(const std::deque<Pair *> &pairs, std::deque<Pair *> &result, bool &freeElementInserted)
{
	for (size_t i = 0; i < pairs.size(); ++i)
		result.push_back(pairs[i]->right);
	
	if (pairs[0]->left)
	{
		result.insert(result.begin(), pairs[0]->left);
		freeElementInserted = true;
	}
}

static void	buildPending(const std::deque<Pair *> &pairs, std::deque<Pair *> &smaller)
{
	for (size_t i = 1; i < pairs.size(); ++i)
		smaller.push_back(pairs[i]->left);
}

static void	buildUpperLimitArray(std::deque<size_t> &upperLimit, const std::deque<Pair *> &smaller, bool freeElementInserted)
{
	size_t i = freeElementInserted ? 2 : 1;
	for (size_t j = 0; j < smaller.size(); ++j)
	{
		upperLimit.push_back(i);
		++i;
	}
}

static size_t	binaryInsertElement(std::deque<Pair *> &resultVector, Pair *elementToInsert, size_t upperLimit)
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

static void	updateUpperBoundAfterInsertion(std::deque<size_t> &upperlimit, size_t insertionPoint)
{
	for (size_t i = 0; i < upperlimit.size(); ++i)
	{
		if (upperlimit[i] >= insertionPoint)
			upperlimit[i]++;
	}
}

// =============================================================================================
// DATA MAPPING FROM INT TO PAIRS AND PAIRS TO INT AND MEMORY MANAGEMENT
// =============================================================================================

static void	intToPairs(const std::deque<int> &initialElements, std::deque<Pair *> &pairs)
{
	for (size_t i = 0; i < initialElements.size(); ++i)
		pairs.push_back(new Pair(initialElements[i]));
}

static std::deque<int> pairsToInt(const std::deque<Pair *> pairs)
{
	std::deque<int> intResult;
	std::deque<Pair *>::const_iterator it;
	for (it = pairs.begin(); it < pairs.end(); ++it)
		intResult.push_back((*it)->value);
	return (intResult);
}


// END







// =============================================================================================
// DEBUGGING PRINT OF TREE AND VECTOR IMPLEMENTATION
// =============================================================================================

std::ostream &operator<<(std::ostream& outstream, std::vector<Pair *> pairs)
{
	std::vector<Pair *>::iterator it;

	//outstream << '\n';
	for (it = pairs.begin(); it < pairs.end(); ++it)
	{
		if (!(*it))
		{
			DEBUG(std::cout << " NULL ";)
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
