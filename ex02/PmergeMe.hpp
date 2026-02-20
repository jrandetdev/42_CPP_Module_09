#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>

#define DEBUG(...)  //__VA_ARGS__
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */
#define RESET   "\033[0m"		/* Reset */
#define MAG		"\033[35m"


template <typename T>
std::ostream &operator<<(std::ostream& outstream, std::vector<T> &vectorContainer)
{
	if (vectorContainer.empty())
	{
		outstream << "empty vector!";
		return outstream;
	}
	for (size_t i = 0; i < vectorContainer.size(); ++i)
	{
		outstream << vectorContainer[i];
		if (i < vectorContainer.size() - 1)
			outstream << " ";
	}
	return outstream;
}

/**
 * @brief Pair class which will contain
 * * two pointers: one to the left pair member and one to the right, with left->value < right->value\
 * * the value is the biggest value of the two 
 * * the canonical form with the default construtor in private (no sense to build a pair of nothing)
 * * a constructor taking a single int to build the leaves of the tree structure in PmergeMe.cpp buildSortedPairTree
 * * a constructor taking two pointers to pair to build the node which will point to its two Pair children
 * 
 */
class	Pair
{
	private:
		Pair(); // no need for the constructor that does not take arguments. 
	public:
		int value;
		Pair *left;
		Pair *right;
		Pair(int value); // what are the various
		Pair(Pair *a, Pair *b);
		~Pair();
};

/**
 * @brief sorts a set of positive integers using the Ford-Johnson algoritm
 * 
 * @param initialElementsVec Reference to the vector to be sorted
 * @return std::vector<int> A new vector with the sorted integers (ascending)
 */
std::vector<int> mergeInsert(std::vector<int> &initialElementsVec);
std::deque<int> mergeInsert(std::deque<int> &initialElements);

std::ostream &operator<<(std::ostream& outstream, std::vector<Pair *> pairs);
void printTree(Pair* root, std::string indent = "", bool isLeft = true);

#endif
