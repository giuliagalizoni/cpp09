#include "../includes/PmergeMe.hpp"

// Default constructor
PmergeMe::PmergeMe() : _time_vec(0.0), _time_deq(0.0), _args(NULL), _count(0), _cmp_vec(0), _cmp_deq(0) {}

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
		_args = other._args;
		_count = other._count;
		_cmp_vec = other._cmp_vec;
		_cmp_deq = other._cmp_deq;
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
	_cmp_vec = 0;
	std::clock_t start = std::clock();
	_vec.clear();
	_vec.reserve(static_cast<size_t>(_count));
	for (int i = 0; i < _count; i++)
		_vec.push_back(std::atoi(_args[i]));
	evaluate_pairs_vec(1);
	std::clock_t end = std::clock();
	_time_vec = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
}

void PmergeMe::sort_deq()
{
	_cmp_deq = 0;
	std::clock_t start = std::clock();
	_deq.clear();
	for (int i = 0; i < _count; i++)
		_deq.push_back(std::atoi(_args[i]));
	evaluate_pairs_deq(1);
	std::clock_t end = std::clock();
	_time_deq = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
}

void PmergeMe::parseArgs(int count, char** args)
{
	if (count < 1)
		throw std::invalid_argument("Error: no arguments");

	// _vec.reserve(static_cast<size_t>(count));
	for (int i = 0; i < count; i++)
	{
		int value = 0;

		if (!validate(args[i], value))
			throw std::invalid_argument(std::string("Error: invalid argument '") + args[i] + "'");
		// _vec.push_back(value);
		// _deq.push_back(value);
	}
	_args = args;
	_count = count;
}

size_t PmergeMe::getVecComparisons() const { return _cmp_vec; }

size_t PmergeMe::getDeqComparisons() const { return _cmp_deq; }
