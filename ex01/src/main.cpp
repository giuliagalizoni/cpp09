#include "../includes/RPN.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	RPN rpn;

	if (argc != 2)
	{
		std::cerr << "Error: wrong number of arguments" << std::endl;
		std::cout << "Usage: ./RPN \"5 1 2 + 4 * + 3\" -" << std::endl;
		return 1;
	}

	try
	{
		int result = rpn.evaluate(argv[1]);
		std::cout << result << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
	return 0;
}
