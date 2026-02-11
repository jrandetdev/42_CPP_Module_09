#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <cstdlib>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREY    "\033[1;30m"


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
Pair	*groupIntoPairs(std::vector<Pair *> pairs);
void	_deleteTree(Pair* node);
void	deleteTree(Pair **nodeRef);


std::ostream &operator<<(std::ostream& outstream, std::vector<int> vecContainer);
std::ostream &operator<<(std::ostream& outstream, std::vector<Pair *> pairs);
void printTree(Pair* root, int space, int count);



#endif
