#include "PmergeMe.hpp"
#include "Output.hpp"

#include <iterator>

//======================= PAIR CLASS METHODS =======================

Pair::Pair() : value(0), left(nullptr), right(nullptr) {}

/**
 * @brief Construct a new Pair:: Pair object with only an int value,
 * the resulting Pair object constructed is a pair with only the value 
 * and left and right pointing to the null address.
 */
Pair::Pair(int value) : value(value), left(nullptr), right(nullptr) {}

/**
 * @brief Construct a new Pair:: Pair object from two pointers to 
 * Pair objects. If the value contained in the left pair (b) is greater than the 
 * value contained in the right pair (a), then the pointers are swaped 
 * and the value contained is the bigger of the two (b)
 * 
 * @param a (pointer to a Pair object)
 * @param b (pointer to a Pair object)
 */
Pair::Pair(Pair *a, Pair *b)
{
	// by defaulét do something, and else swap could be something I could do
	this->value = a->value;
	this->left = b;
	this->right = a;

	// swap them if b is bigger than a
	if (b->value > a->value)
	{
		this->value = b->value;
		this->left = a;
		this->right = b;
	}
}

Pair::~Pair() {}

//===================== MERGE INSERTION START ====================

/**
 * @brief Point of entry of the main sorting algorithm based on the merge insertion sort and 
 * ford johnson sequence.
 * 
 * @param initialElementsVec 
 * @param pairs
 * @return void since the output is directly in thr standard output 
 */
void	mergeInsert(std::vector<int> &initialElementsVec)
{
	std::vector<Pair *> pairs;
	
	intToPair(initialElementsVec, pairs);
	std::cout << pairs << std::endl;
	Pair *root = groupIntoPairs(pairs);
	if (root)
		std::cout << root->value << std::endl;
	deleteTree(&root);
}

/**
 * @brief this function creates the leaves of the tree where all the Pair objects contain 
 * only the value extracted from the input @param initialElementsVec with two nullptr for
 * the left and right children. This will be the last "floor" of the tree
 * 
 * @param initialElementsVec
 * @param 
 */
void	intToPair(const std::vector<int> &initialElementsVec, std::vector<Pair *> &pairs)
{
	for (size_t i = 0; i < initialElementsVec.size(); ++i)
		pairs.push_back(new Pair(initialElementsVec[i]));
}

Pair	*groupIntoPairs(std::vector<Pair *> pairs)
{
	if (pairs.size() <= 1) // just in case hahaha 
	{
		std::cout << "reached the last level where only one pair is." << std::endl;
		return pairs[0];
	}
	std::vector<Pair *> newSequence;
	for (size_t i = 0; i < pairs.size(); i += 2)
	{
		newSequence.push_back(new Pair(pairs[i], pairs[i + 1]));
	}
	std::cout << newSequence << std::endl;
	return (groupIntoPairs(newSequence));
}

void	_deleteTree(Pair* node)
{
	if (node == NULL) return;
	_deleteTree(node->left);
	_deleteTree(node->right);

	std::cout << "Deleting node: " << node->value << std::endl; 
	delete node;
}

//function to delete the tree
void	deleteTree(Pair **nodeRef)
{
	_deleteTree(*nodeRef);
	*nodeRef = NULL;
}
