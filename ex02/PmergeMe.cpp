#include "PmergeMe.hpp"

#include <iterator>
#include <sstream>
#include <iostream>

// ==================== OUTPUT OPERATOR SUBJECT ===========================

std::ostream &operator<<(std::ostream& outstream, std::vector<int> vectorContainer)
{
	//outstream << "Before:	";
	for (size_t i = 0; i < vectorContainer.size(); ++i)
	{
		outstream << vectorContainer[i];
		if (i < vectorContainer.size() - 1)
			outstream << " ";
	}
	return outstream;
}

//==================== DEBUG OUTPUT TO SEE PAIR ====================

// std::ostream &operator<<(std::ostream& outstream, std::vector<Pair *> pairs)
// {
// 	std::vector<Pair *>::iterator it;

// 	outstream << '\n';
// 	for (it = pairs.begin(); it < pairs.end(); ++it)
// 	{
// 		std::string valStr;
// 		std::stringstream ss;
// 		ss << ((*it)->value);
// 		valStr = ss.str();

// 		if ((*it)->left)
// 			outstream << '[' << GREY << "S " << (*it)->left->value << RESET; // Left part
// 		else
// 			outstream << '[' << GREY << "S " << "NULL" << RESET;
// 		outstream << " " << valStr << " ";
// 		if ((*it)->right)
// 			outstream << GREY << "B: " << (*it)->right->value << ']' << RESET;
// 		else
// 			outstream << GREY << "B: " << "NULL" << ']' << RESET;

// 		if (it < pairs.end() - 1)
// 			outstream << "	";
// 	}
// 	return outstream;
// }


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

//======================= PAIR CLASS METHODS =======================

Pair::Pair() : value(0), left(NULL), right(NULL) {}

Pair::Pair(int value) : value(value), left(NULL), right(NULL) {}

Pair::Pair(Pair *a, Pair *b) : value(b->value), left(a), right(b)
{
	if (!a || !b)
		return;
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
	Pair *root;
	
	intToPair(initialElementsVec, pairs);
	groupIntoPairs(initialElementsVec);
}

void	intToPair(const std::vector<int> &initialElementsVec, std::vector<Pair *> &pairs)
{
	for (size_t i = 0; i < initialElementsVec.size(); ++i)
		pairs.push_back(new Pair(initialElementsVec[i]));
}

Pair *groupIntoPairs(std::vector<Pair *> pairs)
{
	std::vector<Pair *> treeFloor;
	std::vector<Pair *>::iterator it;
	std::vector<Pair *>::iterator lastElement;

	if (pairs.size() == 1)
	{
		std::cout << GREEN << "\nthe top of my recursion tree" << RESET << std::endl;
		return (pairs[0]);
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

void	sortTree(Pair *root)
{
	std::vector<int> result;
	std::vector<Pair *>::iterator it;

	(void)root;
	// Build the result or main chain with the winners of the case
	for (it = pairs.begin(); it != pairs.end(); ++it)
		result.push_back((*it)->right->value);

	// if (pairs[0]->left)
	// 	result.insert(pairs.begin(), pairs[0]->left->value);

	// std::cout << result << std::endl;

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
