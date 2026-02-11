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

		if ((*it)->left)
			outstream << '[' << GREY << (*it)->left->value << RESET; // Left part
		else
			outstream << '[' << GREY << "NULL" << RESET;
		outstream << " " << valStr << " ";
		if ((*it)->right)
			outstream << GREY << (*it)->right->value << ']' << RESET;
		else
			outstream << GREY << "NULL" << ']' << RESET;

		if (it < pairs.end() - 1)
			outstream << "	";
	}
	return outstream;
}

// void printTree(Pair* root, int space, int count)
// {
//     if (root == NULL) return;

//     // Increase distance between levels
//     space += count;

//     // Process right child first
//     printTree(root->right, space, count);

//     // Print current node after space
//     std::cout << std::endl;
//     for (int i = count; i < space; i++) std::cout << " ";
//     std::cout << root->value << "\n";

//     // Process left child
//     printTree(root->left, space, count);
// }

//======================= PAIR CLASS METHODS =======================

Pair::Pair() : value(0), left(NULL), right(NULL) {}

Pair::Pair(int value) : value(value), left(NULL), right(NULL) {}

Pair::Pair(Pair *a, Pair *b)
{
	if (!a)
	{
		this->value = b->value;
		this->left = NULL;
		this->right = b;
		return;
	}
	this->value = b->value;
	this->left = a;
	this->right = b;

	if (a->value > b->value)
	{
		this->value = a->value;
		this->left = b;
		this->right = a;
	}
}

Pair::~Pair() {}

//===================== MERGE INSERTION START ====================

void	mergeInsert(std::vector<int> &initialElementsVec)
{
	std::vector<Pair *> pairs;
	
	intToPair(initialElementsVec, pairs);
	std::cout << pairs << std::endl;
	groupIntoPairs(pairs);
	//printTree(root, 0, 5);
}

void	intToPair(const std::vector<int> &initialElementsVec, std::vector<Pair *> &pairs)
{
	for (size_t i = 0; i < initialElementsVec.size(); ++i)
		pairs.push_back(new Pair(initialElementsVec[i]));
}

void groupIntoPairs(std::vector<Pair *> pairs)
{
	std::vector<Pair *> treeFloor;
	std::vector<Pair *>::iterator it;
	std::vector<Pair *>::iterator lastElement;

	if ((pairs.size() % 2) == 0)
		lastElement = pairs.end();
	else
		lastElement = pairs.end() - 1;

	for (it = pairs.begin(); it != lastElement; it += 2)
		treeFloor.push_back(new Pair(*it, *(it + 1)));

	if (lastElement != pairs.end())
		treeFloor.push_back(new Pair(NULL, *lastElement));

	std::cout << treeFloor << std::endl;
	if (pairs.size() > 1)
		return (groupIntoPairs(treeFloor));
	sortTree(treeFloor);
}

void	sortTree(std::vector<Pair *> pairs)
{
	std::cout << "inside the sortTree we get the root node of the tree" << pairs << std::endl;
	deleteTree(&pairs[0]);
}

void	_deleteTree(Pair* node)
{
	if (node == NULL) return;
	_deleteTree(node->left);
	_deleteTree(node->right);
	delete node;
}

//function to delete the tree
void	deleteTree(Pair **nodeRef)
{
	_deleteTree(*nodeRef);
	*nodeRef = NULL;
}
