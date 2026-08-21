#include "../includes/RPN.hpp"
#include <iostream>

int main()
{
	std::string tests[] = {
		"3 4 +",       // 7
		"5 1 2 + 4 * + 3 -", // 14
		"7 2 -",       // 5
		"1 2 * 3 4 * +", // 14
		"5 6 -",       // -1 (valid, tests the -1-as-error ambiguity)
		"5 0 /",       // division by zero
		"3 +",         // not enough operands
		"3 4",         // leftover values
		"3 12",        // invalid token (multi-digit)
		"",            // empty expression
	};
	int n = sizeof(tests) / sizeof(tests[0]);

	for (int i = 0; i < n; i++)
	{
		std::cout << "\"" << tests[i] << "\" -> ";
		try
		{
			int result = evaluate(tests[i]);
			std::cout << result << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << "exception: " << e.what() << std::endl;
		}
	}
	return 0;
}
