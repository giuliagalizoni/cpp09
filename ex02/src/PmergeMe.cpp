#include "../includes/PmergeMe.hpp"

// Default constructor
PmergeMe::PmergeMe() : _time_vec(0.0), _time_deq(0.0) {}

// Copy constructor
PmergeMe::PmergeMe(const PmergeMe& other)
{
	*this = other;
}

// Copy assignment operator
PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		_vec = other._vec;
		_deq = other._deq;
		_time_vec = other._time_vec;
		_time_deq = other._time_deq;
	}
	return *this;
}

// Destructor
PmergeMe::~PmergeMe() {}

const std::vector<int>& PmergeMe::getVec() const { return _vec; }

double PmergeMe::getVecTime() const { return _time_vec; }

const std::deque<int>&  PmergeMe::getDeq() const { return _deq; }

double PmergeMe::getDeqTime() const { return _time_deq; }

void PmergeMe::sort_vec()
{
	std::clock_t start = std::clock();
	evaluate_pairs_vec(1);
	std::clock_t end = std::clock();
	_time_vec = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
}

void PmergeMe::sort_deq()
{
	std::clock_t start = std::clock();
	evaluate_pairs_deq(1);
	std::clock_t end = std::clock();
	_time_deq = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
}

void PmergeMe::parseArgs(int count, char** args)
{
	if (count < 1)
		throw std::invalid_argument("Error: no arguments");

	_vec.reserve(static_cast<size_t>(count));
	for (int i = 0; i < count; i++)
	{
		int value = 0;

		if (!validate(args[i], value))
			throw std::invalid_argument(std::string("Error: invalid argument '") + args[i] + "'");
		_vec.push_back(value);
		_deq.push_back(value);
	}
}
