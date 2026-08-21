#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <string>
# include <list>
# include <sstream>
# include <cstdlib>

class RPN
{
	public:
		RPN();
		RPN(const RPN& other);
		RPN& operator=(const RPN& other);
		~RPN();

		int evaluate(const std::string &expr);

		class InvalidToken : public std::exception
		{
			public:
				const char *what() const throw();
		};

		class NotEnoughOperands : public std::exception
		{
			public:
				const char *what() const throw();
		};

		class TooManyOperands : public std::exception
		{
			public:
				const char *what() const throw();
		};

		class EmptyResult : public std::exception
		{
			public:
				const char *what() const throw();
		};

		class DivisionByZero : public std::exception
		{
			public:
				const char *what() const throw();
		};
};

#endif
