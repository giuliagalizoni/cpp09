#include "../includes/PmergeMe.hpp"

bool PmergeMe::validate(const std::string &arg, int &value) const
{
	if (arg.empty())
		return false;

	long n = 0;
	for (size_t i = 0; i < arg.size(); i++)
	{
		if (!std::isdigit(static_cast<unsigned char>(arg[i])))
			return false;

		int digit = arg[i] - '0';
		if (n > (INT_MAX - digit) / 10)
			return false;
		n = n * 10 + digit;
	}

	if (n == 0)
		return false;

	value = static_cast<int>(n);
	return true;
}

/*
Generates the Jacobsthal numbers used to order loser insertions.
Recurrence:  J(k) = J(k-1) + 2 * J(k-2),  seeded with 1 and 3
			-> 1, 3, 5, 11, 21, 43, 85, ...
			(equivalent to Knuth's t_k = 2^k - t_(k-1), t_0 = 1)*/
std::vector<int> PmergeMe::jacobsthal(int n) const
{
	std::vector<int> seq;

	if (n < 1)
		return seq;

	seq.push_back(1);

	if (n < 2)
		return seq;

	seq.push_back(3);

	while (seq[seq.size() - 1] < n)
	{
		int next = seq[seq.size() - 1] + 2 * seq[seq.size() - 2];
		seq.push_back(next);
	}
	return seq;
}

/*
Returns the 1-based pair indices of the losers, in Jacobsthal insertion order.
b1 is absent — it is pre-placed at the head of the main chain.
insertion_order(10) -> 3 2  5 4  10 9 8 7 6
*/
std::vector<int> PmergeMe::insertion_order(int nPairs) const
{
	std::vector<int> order;

	if (nPairs <= 1)
		return order;

	std::vector<int> jac = jacobsthal(nPairs);
	for (size_t g = 1; g < jac.size(); ++g)
	{
		int hi = jac[g] < nPairs ? jac[g] : nPairs;
		int lo = jac[g - 1] + 1;

		if (lo > nPairs)
			break;
		for (int i = hi; i >= lo; --i)
			order.push_back(i);
	}
	return order;
}
