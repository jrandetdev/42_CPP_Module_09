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

std::ostream &operator<<(std::ostream& outstream, std::vector<Pair *> pairs)
{
	std::vector<Pair *>::iterator it;

	outstream << '\n';
	for (it = pairs.begin(); it < pairs.end(); ++it)
	{
		if (!(*it))
		{
			std::cout << "NULL";
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

void	insertElementInResult(std::vector<Pair *> &resultVector, Pair *elementToInsert)
{
	if (!elementToInsert)
		return ;
	std::vector<Pair *>::iterator low = resultVector.begin();
	std::vector<Pair *>::iterator high = resultVector.end();
	size_t difference;

	while (low < high)
	{
		difference = high - low;
		//std::cout << difference << std::endl;
		std::vector<Pair*>::iterator mid = low + difference / 2;
		if (elementToInsert->value >= (*mid)->value)
			low = mid + 1;
		else
			high = mid;
	}
	resultVector.insert(low, elementToInsert);
}

std::vector<Pair *>	sortTree(std::vector<Pair *> pairs)
{
	if (pairs[0]->right == NULL)
		return pairs;
	std::vector<Pair *> result;
	for (size_t i = 0; i < pairs.size(); ++i)
		result.push_back(pairs[i]->right);

	// free element to insert
	if (pairs[0]->left)
		result.insert(result.begin(), pairs[0]->left);

	std::vector<Pair *> smaller;
	for (size_t i = 1; i < pairs.size(); ++i)
		smaller.push_back(pairs[i]->left);

	std::cout << "\n\n\nresult: " << result << std::endl;
	std::cout << "\nsmaller: " << smaller << std::endl;

	std::vector<Pair *>::iterator it;
	for (it = smaller.begin(); it != smaller.end(); ++it)
		insertElementInResult(result, *it);
	
	std::cout << "\nafter insert sort, result: " << result << std::endl;

	return (sortTree(result));
	// for each pair within the tree, push back the child ont he right in the result vector 

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

void	intToPair(const std::vector<int> &initialElementsVec, std::vector<Pair *> &pairs)
{
	for (size_t i = 0; i < initialElementsVec.size(); ++i)
		pairs.push_back(new Pair(initialElementsVec[i]));
}

void	mergeInsert(std::vector<int> &initialElementsVec)
{
	std::vector<Pair *> pairs;
	std::vector<Pair *> dummy;
	
	intToPair(initialElementsVec, pairs);
	dummy = groupIntoPairs(pairs);
	sortTree(dummy);
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
