#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <iomanip>


// so where do I build the vector with ints and the dequeu?
// probaby in the main directly and then they will be send via reference or pointer 


class	Pair
{
	private:
		// what makes sense to put in private here? in my program, outsid eof my functions, wil i need to construct a pair?
	public:
		unsigned int value;
		Pair *left;
		Pair *right;
		Pair(unsigned int value); // what are the various
		Pair(Pair *a, Pair *b);
		Pair(Pair &a, Pair &b);

		// I will need to build a pair based on the comparison
		// Overload the comparison operators for the pairs <= >= == to compare them
};

std::ostream& operator<<(std::ostream& out, const Pair& pairInstance);


#endif
