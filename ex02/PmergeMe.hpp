#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <cstdlib>

#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */
#define RESET   "\033[0m"		/* Reset */
#define MAG		"\033[35m"


// so where do I build the vector with ints and the dequeu?
// probaby in the main directly and then they will be send via reference or pointer 

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
		Pair(Pair &a, Pair &b);
		~Pair();

		// I will need to build a pair based on the comparison
		// Overload the comparison operators for the pairs <= >= == to compare them
};

void	mergeInsert(std::vector<int> &initialElementsVec);
void	intToPair(const std::vector<int> &initialElementsVec, std::vector<Pair *> &pairs);
std::vector<Pair *> groupIntoPairs(std::vector<Pair *> pairs);
std::vector<Pair *>	sortTree(std::vector<Pair *> pairs);
// void	_deleteTree(Pair* node);
// void	deleteTree(Pair **nodeRef);


std::ostream &operator<<(std::ostream& outstream, std::vector<int> vecContainer);
std::ostream &operator<<(std::ostream& outstream, std::vector<Pair *> pairs);
void printTree(Pair* root, std::string indent = "", bool isLeft = true);




#endif
