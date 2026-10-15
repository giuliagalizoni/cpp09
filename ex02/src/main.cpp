#include "../includes/PmergeMe.hpp"

#include <iomanip>

template <typename Container>
static void printSequence(const Container& c)
{
	typename Container::const_iterator it = c.begin();
	while (it != c.end())
	{
		if (it != c.begin())
			std::cout << " ";
		std::cout << *it;
		++it;
	}
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	PmergeMe p;

	try
	{
		p.parseArgs(argc - 1, argv + 1);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	std::cout << "Before: ";
	for (int i = 1; i < argc; i++)
		std::cout << " " << argv[i];
	std::cout << std::endl;

	p.sort_vec();
	p.sort_deq();

	std::cout << "After (vec):  ";
	printSequence(p.getVec());
	std::cout << "After (deq):  ";
	printSequence(p.getDeq());

	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << p.getVec().size()
				<< " elements with std::vector : " << p.getVecTime() << " us" << std::endl;
	std::cout << "Time to process a range of " << p.getDeq().size()
				<< " elements with std::deque  : " << p.getDeqTime() << " us" << std::endl;

	std::cout << "Comparisons with std::vector : " << p.getVecComparisons() << std::endl;
	std::cout << "Comparisons with std::deque  : " << p.getDeqComparisons() << std::endl;

	return 0;
}
