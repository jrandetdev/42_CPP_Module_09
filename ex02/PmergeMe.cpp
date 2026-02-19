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

// Main logic of SortAndInsertByJacobStahl
static void					buildMainChain(const std::vector<Pair *> &pairs, std::vector<Pair *> &result, bool &freeElementInserted);
static void					buildPending(const std::vector<Pair *> &pairs, std::vector<Pair *> &smaller);
static void					buildUpperLimitArray(std::vector<size_t> &upperLimit, const std::vector<Pair *> &smaller, bool freeElementInserted);
static void					generateJacobIndexes(size_t size, std::vector<int> &jacobIndexes);
static int					getMinIndex(int a, int b);
static size_t				insertPendingElement(std::vector<Pair *> &resultVector, Pair *elementToInsert, size_t upperLimit);
static void					updateUpperBound(std::vector<size_t> &upperlimit, size_t insertionPoint);

// =============================================================================
// MERGE INSERTION LOGIC FOR STD::VECTOR
// =============================================================================

std::vector<int> mergeInsert(std::vector<int> &initialElementsVec)
{
	pairCompCounter = 0;
	insertCompCounter = 0;
	
	std::vector<Pair *> pairs;
	std::vector<Pair *> root;
	std::vector<Pair *> result;
	std::vector<int> finalResult;
	
	intToPairs(initialElementsVec, pairs);		// Transform the initial list into the Pair format (Pair being the class I have)
	root = buildSortedPairTree(pairs);				// 
	result = sortAndInsertByJacobStahl(root);
	finalResult = pairsToInt(result);
	
	DEBUG(std::cout << "Comparisons made during pair making: " << pairCompCounter)
	DEBUG(<< " and comparisons made during insertion " << insertCompCounter)
	DEBUG(<< " and total: " << pairCompCounter + insertCompCounter)
	DEBUG(<< std::endl;)
	DEBUG(std::cout << "Ideal number of comparisons for " << initialElementsVec.size()
	<< " is " << idealComparisonNumber(initialElementsVec.size())
	<< std::endl;)
	deleteTree(&root[0]);
	return (finalResult);
}

//	Utility for seeing if implementation meets requirements 
int idealComparisonNumber(int numberOfElements)
{
	int sum = 0;
	for (int k = 1; k <= numberOfElements; ++k) {
		double value = (3.0 / 4.0) * k;
		sum += static_cast<int>(std::ceil(log2(value)));
	}
	return sum;
}

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
		treeFloor.push_back(new Pair(NULL, *lastElement));

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
	// Stop condition of the recursion, the right element is NULL meaning I have hit a node where left and right are NULL
	if (pairs[0]->right == NULL)
		return pairs;

	std::vector<Pair *> result;
	std::vector<Pair *> smaller;
	std::vector<size_t> upperLimit;
	bool freeElementInserted = false;

	buildMainChain(pairs, result, freeElementInserted);
	buildPending(pairs, smaller);
	buildUpperLimitArray(upperLimit, smaller, freeElementInserted);

	DEBUG(std::cout << "\nresult array: " << result << std::endl;)
	DEBUG(std::cout << "smaller elements to be inserted in result: " << smaller << std::endl;)
	DEBUG(std::cout << "upperlimit array: " << upperLimit << std::endl;)
	
	std::vector<int> jacobIndexes(smaller.size());
	generateJacobIndexes(smaller.size(), jacobIndexes);
	size_t insertionPoint;

	for (size_t i = 0; i < smaller.size(); ++i)
	{
		int index = jacobIndexes[i];
		if (smaller[index])
		{
			insertionPoint = insertPendingElement(result, smaller[index], upperLimit[index]);
			updateUpperBound(upperLimit, insertionPoint);
			DEBUG(std::cout << "inserted the number " << smaller[index]->value << std::endl;)
			DEBUG(std::cout << "upperlimit array: " << upperLimit << std::endl;)
			DEBUG(std::cout << "result array: " << result << std::endl;)
		}
	}
	
	DEBUG(std::cout << "\nafter insert sort, result: " << result << std::endl;)
	return (sortAndInsertByJacobStahl(result));
}

// MATHEMATICAL SEQUENCE GENERATOR, SORTS THE SMALLER ELEMENTS ACCORDING TO THIS ORDER FOR THE BINARY INSERT METHOD (LINEAR)
static void generateJacobIndexes(size_t size, std::vector<int> &jacobIndexes) {
	// std::vector<int> indexes(size);
	const int jacobSequence[] = {1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203};
	
	int jacobIndex = 1;		// jacobIndex starts at 1 because we start at 3 in the sequence
	size_t index = 0;		// index used to go through the jacobSequence
	
	while (index < size)	// going through the jacobsequence as long as index is smaller than size
	{
		int number = getMinIndex(jacobSequence[jacobIndex] - 2, static_cast<int>(size - 1));	// treat the edge case of having 13 elements to sort, the next jacobSequence[jacobIndex] would be 21 which does not work. so we calculate the min
		int previousNumber = jacobSequence[jacobIndex - 1] - 2;
		while (number > previousNumber)	// 
		{
			jacobIndexes[number] = index; // 0
			index++; // 1
			number--;
		}
		jacobIndex++;
	}
}

static int	getMinIndex(int a, int b)
{
	return (a < b ? a : b);
}

// ===========================================================================================================================================================
// INSERTING FUNCTIONS THAT MANIPULATE THE CONTAINERS FOR THE MAIN CHAIN, PENDING CHAIN, AND UPPERLIMIT INDEX ARRAY USED TO OPTIMISE THE AMOUNT OF COMPARISONS
// ===========================================================================================================================================================

static void	buildMainChain(const std::vector<Pair *> &pairs, std::vector<Pair *> &result, bool &freeElementInserted)
{
	for (size_t i = 0; i < pairs.size(); ++i)
		result.push_back(pairs[i]->right);
	
	if (pairs[0]->left)
	{
		result.insert(result.begin(), pairs[0]->left);
		freeElementInserted = true;
	}
}

static void	buildPending(const std::vector<Pair *> &pairs, std::vector<Pair *> &smaller)
{
	for (size_t i = 1; i < pairs.size(); ++i)
		smaller.push_back(pairs[i]->left);
}

static void	buildUpperLimitArray(std::vector<size_t> &upperLimit, const std::vector<Pair *> &smaller, bool freeElementInserted)
{
	size_t i = freeElementInserted ? 2 : 1;
	for (size_t j = 0; j < smaller.size(); ++j)
	{
		upperLimit.push_back(i);
		++i;
	}
}

static size_t	insertPendingElement(std::vector<Pair *> &resultVector, Pair *elementToInsert, size_t upperLimit)
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
			insertCompCounter++;
		}
		else
		{
			high = mid; // the reason for the +1 not here is because of the exclusion [ ( of the insert function.
			insertCompCounter++;
		}
	}
	insertionPoint = low - resultVector.begin();	
	resultVector.insert(low, elementToInsert);
	return (insertionPoint);
}

static void	updateUpperBound(std::vector<size_t> &upperlimit, size_t insertionPoint)
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
static size_t	insertPendingElement(std::deque<Pair *> &resultVector, Pair *elementToInsert, size_t upperLimit);
static void	updateUpperBound(std::deque<size_t> &upperlimit, size_t insertionPoint);


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
			insertionPoint = insertPendingElement(result, smaller[index], upperLimit[index]);
			updateUpperBound(upperLimit, insertionPoint);
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

static size_t	insertPendingElement(std::deque<Pair *> &resultVector, Pair *elementToInsert, size_t upperLimit)
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

static void	updateUpperBound(std::deque<size_t> &upperlimit, size_t insertionPoint)
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
