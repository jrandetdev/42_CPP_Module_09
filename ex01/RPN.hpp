#ifndef RPN_HPP
# define RPN_HPP

#include <stack>
#include <algorithm>

class RPN
{
	private:
		std::stack<int> reversePolishNotation;
	public:
		RPN();
		RPN(const RPN& other);
		RPN operator=(const RPN& other);
		~RPN();
};

#endif
