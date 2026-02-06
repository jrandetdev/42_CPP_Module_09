#include "PmergeMe.hpp"

#include <iterator>
#include <sstream>
#include <iostream>

// ==================== OUTPUT OPERATOR SUBJECT ===========================

std::ostream &operator<<(std::ostream& outstream, std::vector<int> vectorContainer)
{
	outstream << "Before:	";
	for (size_t i = 0; i < vectorContainer.size(); ++i)
	{
		outstream << vectorContainer[i];
		if (i < vectorContainer.size() - 1)
			outstream << " ";
	}
	return outstream;
}

//==================== DEBUG OUTPUT TO SEE PAIR ====================

std::ostream &operator<<(std::ostream& outstream, std::vector<Pair *> pairs)
{
	std::vector<Pair *>::iterator it;

	outstream << '\n';
	for (it = pairs.begin(); it < pairs.end(); ++it)
	{
		std::string valStr;
		std::stringstream ss;
		ss << ((*it)->value);
		valStr = ss.str();

		int totalWidth = 11;
		int padding = (totalWidth - valStr.length()) / 2;

		if ((*it)->left)
			outstream << '[' << std::setw(2) << (*it)->left; // Left part
		else
			outstream << '[' << std::setw(2) << "NULL";
		outstream << std::string(padding, ' ') << valStr 
				<< std::string(totalWidth - padding - valStr.length(), ' ');

		if ((*it)->right)
			outstream << std::setw(2) << (*it)->right << '|';
		else
			outstream << std::setw(2) << "NULL" << '|';

		if (it < pairs.end() - 1)
			outstream << "	";
	}
	return outstream;
}

void printTree(Pair* root, int space, int count)
{
    if (root == NULL) return;

    // Increase distance between levels
    space += count;

    // Process right child first
    printTree(root->right, space, count);

    // Print current node after space
    std::cout << std::endl;
    for (int i = count; i < space; i++) std::cout << " ";
    std::cout << root->value << "\n";

    // Process left child
    printTree(root->left, space, count);
}

//======================= PAIR CLASS METHODS =======================

Pair::Pair() : value(0), left(NULL), right(NULL) {}

/**
 * @brief Construct a new Pair:: Pair object with only an int value,
 * the resulting Pair object constructed is a pair with only the value 
 * and left and right pointing to the null address.
 */
Pair::Pair(int value) : value(value), left(NULL), right(NULL) {}

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

	// by default I take the one on the right as being the biggest one
	// avoids unnecessary operations
	this->value = b->value;
	this->left = a;
	this->right = b;

	// swap them if a (one on the right) is bigger than b (ont on the left)
	if (a->value > b->value)
	{
		this->value = a->value;
		this->left = b;
		this->right = a;
	}
	// i will need to perform a check to see if they exist or not
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
	// if (root)
	// 	std::cout << root->value << std::endl;
	printTree(root, 0, 5);
	deleteTree(&root);
}

/**
 * @brief this function creates the leaves of the tree where all the Pair objects contain 
 * only the value extracted from the input @param initialElementsVec with two NULL for
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
	std::vector<Pair *>::iterator it;
	std::vector<Pair *>::iterator lastElement;
	// This is to determine the last element since we will be advancing by i += 2 (we need to
	// stop before in order not to segfault
	pairs.size() % 2 ? lastElement = pairs.end() - 1 : lastElement = pairs.end();
	for (it = pairs.begin(); it != lastElement; it += 2)
	{
		newSequence.push_back(new Pair(*it, *(it + 1)));
	}
	if (lastElement != pairs.end())
	{
		std::cout << "entered the case of the uneven sequence" << std::endl;
		newSequence.push_back(new Pair(NULL, *lastElement));
	}
	// for (size_t i = 0; i < pairs.size(); i += 2)
	// {
	// 	std::cout << "building a floor..." << std::endl;

	// 	newSequence.push_back(new Pair(pairs[i], pairs[i + 1]));
	// }
	return (groupIntoPairs(newSequence)); // call back group into pairs, if there is an odd amount of pair
	// then it is just not grouped into pairs
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
