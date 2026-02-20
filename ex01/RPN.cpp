#include "RPN.hpp"

RPN::RPN() : tokenStack(), m_rhsOperand(0), m_lhsOperand(0)
{} 

RPN::RPN(const std::string& expression) : tokenStack(), m_rhsOperand(0), m_lhsOperand(0)
{
	std::string validChars = "0123456789+*-/ ";

	for (size_t i = 0; i < expression.length(); ++i)
	{
		if (validChars.find(expression[i]) == std::string::npos)
			throw std::runtime_error("Error, invalid char that does not belong to the following: 0123456789+*-/ ");

		switch(expression[i]) {
			case '+':
				getOperands();
				tokenStack.push(m_lhsOperand + m_rhsOperand);
				break;
			case '-':
				getOperands();
				tokenStack.push(m_lhsOperand - m_rhsOperand);
				break;
			case '/':
				getOperands();
				if (m_rhsOperand == 0)
					throw std::runtime_error("Error: division by zero is not possible.");
				tokenStack.push(m_lhsOperand / m_rhsOperand);
				break;
			case '*':
				getOperands();
				tokenStack.push(m_lhsOperand * m_rhsOperand);
				break;
			case ' ':
				break;
			default:
				tokenStack.push((expression[i] - '0'));
				break;
		}
	}
	// Check that only the result remains
	if (tokenStack.size() != 1)
		throw std::runtime_error("Error: invalid expression remains that cannot be calculated.");
	// Otherwise push the result to the standard output
	std::cout << tokenStack.top() << std::endl;
}

RPN::RPN(const RPN& other) : tokenStack(other.tokenStack),
m_rhsOperand(other.m_rhsOperand), m_lhsOperand(other.m_lhsOperand)
{}

RPN RPN::operator=(const RPN& other)
{
	if (this != &other)
	{
		this->tokenStack = other.tokenStack;
		this->m_rhsOperand = other.m_rhsOperand;
		this->m_lhsOperand = other.m_lhsOperand;
	}
	return (*this);
}

RPN::~RPN() {}

void	RPN::getOperands()
{
	m_rhsOperand = tokenStack.top();
	tokenStack.pop();
	// 
	if (tokenStack.empty())
		throw std::runtime_error("Error, incomplete expression: need two operands when the operator is called.");
	m_lhsOperand = tokenStack.top();
	tokenStack.pop();
}
