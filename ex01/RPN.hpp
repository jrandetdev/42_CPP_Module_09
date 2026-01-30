#ifndef RPN_HPP
# define RPN_HPP

#include <iostream>
#include <stack>
#include <algorithm>
#include <string>
#include <exception>

class RPN
{
	private:
		std::stack<int> tokenStack;
		int	m_rhsOperand;
		int m_lhsOperand;
	public:
		RPN();
		RPN(const std::string& expression);
		RPN(const RPN& other);
		RPN operator=(const RPN& other);
		~RPN();

		void getOperands();

};

#endif
