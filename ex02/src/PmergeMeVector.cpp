#include "../includes/PmergeMe.hpp"

int PmergeMe::get_key_vec(int i, int pairSize) const
{
	int end = (i + 1) * pairSize - 1;

	return _vec[end];
}

void PmergeMe::swap_groups_vec(int iA, int iB, int pairSize)
{
	int startA = iA * pairSize;
	int startB = iB * pairSize;

	for (int i = 0; i < pairSize; i++)
		std::swap(_vec[startA + i], _vec[startB + i]);
}

/*
Ford-Johnson recursion (vector version).
Going down: compare neighboring groups by their key (last number) and swap
so the smaller group (loser) comes first and the bigger one (winner)
second. Then recurse with pairSize * 2, so each (loser, winner) pair is
treated as one bigger group. The next level pairs up those pairs and
compares them by their winners.
Stop when fewer than 2 groups are left (nothing to pair).
Going back up: after each recursive call returns, the winners of this
level are sorted, so insert_loosers_vec(pairSize) inserts this level's
losers into them. The last insertion, with pairSize = 1, gives the fully
sorted result.
*/
void PmergeMe::evaluate_pairs_vec(int pairSize)
{
	int nGroups = _vec.size() / pairSize;

	if (nGroups < 2)
		return;

	for (int i = 0; i < nGroups - 1; i += 2)
	{
		_cmp_vec++;
		if (get_key_vec(i, pairSize) > get_key_vec(i + 1, pairSize))
			swap_groups_vec(i, i + 1, pairSize);
	}

	evaluate_pairs_vec(pairSize*2);
	insert_loosers_vec(pairSize);
}

int PmergeMe::binarySearch(const std::vector<int> &main, int key, int bound) const
{
	int low = 0;
	int high = bound;

	while (low < high)
	{
		int mid = (low + high) / 2;
		_cmp_vec++;
		if (main[mid] < key)
			low = mid + 1;
		else
			high = mid;
	}
	return low;
}

void PmergeMe::append_group_vec(std::vector<int> &dest, int i, int pairSize) const
{
	for (int j = 0; j < pairSize; j++)
		dest.push_back(_vec[i * pairSize + j]);
}

/*
Ford-Johnson insertion step for one recursion level (vector version).

The numbers are split into groups of pairSize. Neighboring groups form
pairs: the loser (b) first, the winner (a) second. The winners are already
sorted. The last number of a group is its key, used for comparisons.
Numbers that do not fill a group at the end are the tail.

1. Build the main chain: b1 first, then every winner (a1, a2, ...).
   They are only appended, no search needed. keys stores each group's key.
2. Insert the other losers (b2, b3, ... and the straggler if there is one)
   in Jacobsthal order. For each loser:
     - binary search its key in keys, but only before its winner's
       position (pos[t]), because a loser is always smaller than its winner
     - insert the whole group in the main chain at that position
     - add one to pos for every winner that moved one slot to the right
3. Append the tail and copy the main chain back to _vec.

The losers are read directly from _vec by group index (2 * t - 2).
_vec is not changed until the very end.
*/
void PmergeMe::insert_loosers_vec(int pairSize)
{
	std::vector<int> mainChain;
	std::vector<int> keys;
	int nGroups = _vec.size() / pairSize;

	append_group_vec(mainChain, 0, pairSize);
	keys.push_back(get_key_vec(0, pairSize));
	for (int i = 1; i < nGroups; i += 2)
	{
		append_group_vec(mainChain, i, pairSize);
		keys.push_back(get_key_vec(i, pairSize));
	}

	int pairs = nGroups / 2;
	int nLosers = pairs + (nGroups % 2);
	std::vector<int> pos(nLosers + 1);
	for (int i = 1; i <= nLosers; i++)
		pos[i] = i;

	std::vector<int> order = insertion_order(nLosers);
	for (size_t k = 0; k < order.size(); k++)
	{
		int t = order[k];
		int srcGroup = 2 * t - 2;
		int key = get_key_vec(srcGroup, pairSize);
		int q = binarySearch(keys,key, pos[t]);
		mainChain.insert(mainChain.begin() + q * pairSize,
				_vec.begin() + srcGroup * pairSize,
				_vec.begin() + (srcGroup + 1) * pairSize);
		keys.insert(keys.begin() + q, key);
		for (int j = 1; j <= nLosers; j++)
		{
			if (pos[j] >= q)
				pos[j]++;
		}
	}

	for (size_t i = static_cast<size_t>(nGroups) * pairSize; i < _vec.size(); i++)
		mainChain.push_back(_vec[i]);

	_vec = mainChain;
}
