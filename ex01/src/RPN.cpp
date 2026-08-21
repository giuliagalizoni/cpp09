#include "RPN.hpp"

// Default constructor
RPN::RPN() {}

// Copy constructor
RPN::RPN(const RPN& other)
{
	*this = other;
}

// Copy assignment operator
RPN& RPN::operator=(const RPN& other)
{
	(void)other;
	return *this;
}

// Destructor
RPN::~RPN() {}

int RPN::evaluate(const std::string &expr)
{
	std::list<int> stack;
	std::istringstream iss(expr);
	std::string token;

	while (iss >> token)
	{
		if (token.size() != 1)
			throw InvalidToken();
		char c = token[0];
		if (std::isdigit(static_cast<unsigned char>(c)))
		{
			stack.push_back(c - '0');
			continue;
		}
		switch (c)
		{
			case '+': case '-': case '*': case '/':
				break;
			default:
				throw InvalidToken();
		}
		if (stack.size() < 2)
			return -1;
		int right = stack.back(); // take the last value
		stack.pop_back(); // revome the last value
		int left = stack.back();
		stack.pop_back();
		int result;
		switch (c)
		{
			case '+': result = left + right; break;
			case '-': result = left - right; break;
			case '*': result = left * right; break;
			case '/':
				if (right == 0) throw DivisionByZero();
				result = left / right;
				break;
		}
		stack.push_back(result);
	}
	if (stack.size() == 0)
		throw EmptyResult();
	if (stack.size() > 1)
		throw TooManyOperands();
	return stack.back();
}


const char *RPN::InvalidToken::what() const throw()
{
	return "Error: Invalid token";
}



const char *RPN::NotEnoughOperands::what() const throw()
{
	return "Error: not enough operands to perform operation";
}

const char *RPN::TooManyOperands::what() const throw()
{
	return "Error: too many operands left over";
}


const char *RPN::EmptyResult::what() const throw()
{
	return "Error: empty result";
}

const char *RPN::DivisionByZero::what() const throw()
{
	return "Error: division by 0 is not possible";
}
