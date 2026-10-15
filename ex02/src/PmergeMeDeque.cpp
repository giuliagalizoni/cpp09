#include "../includes/PmergeMe.hpp"

int PmergeMe::get_key_deq(int i, int pairSize) const
{
	int end = (i + 1) * pairSize - 1;

	return _deq[end];
}

void PmergeMe::swap_groups_deq(int iA, int iB, int pairSize)
{
	int startA = iA * pairSize;
	int startB = iB * pairSize;

	for (int i = 0; i < pairSize; i++)
		std::swap(_deq[startA + i], _deq[startB + i]);
}

/*
Ford-Johnson recursion (deque version).
Going down: compare neighboring groups by their key (last number) and swap
so the smaller group (loser) comes first and the bigger one (winner)
second. Then recurse with pairSize * 2, so each (loser, winner) pair is
treated as one bigger group. The next level pairs up those pairs and
compares them by their winners.
Stop when fewer than 2 groups are left (nothing to pair).
Going back up: after each recursive call returns, the winners of this
level are sorted, so insert_loosers_deq(pairSize) inserts this level's
losers into them. The last insertion, with pairSize = 1, gives the fully
sorted result.
*/
void PmergeMe::evaluate_pairs_deq(int pairSize)
{
	int nGroups = _deq.size() / pairSize;

	if (nGroups < 2)
		return;

	for (int i = 0; i < nGroups - 1; i += 2)
	{
		_cmp_deq++;
		if (get_key_deq(i, pairSize) > get_key_deq(i + 1, pairSize))
			swap_groups_deq(i, i + 1, pairSize);
	}

	evaluate_pairs_deq(pairSize*2);
	insert_loosers_deq(pairSize);
}

int PmergeMe::binarySearch(const std::deque<int> &main, int key, int bound) const
{
	int low = 0;
	int high = bound;

	while (low < high)
	{
		int mid = (low + high) / 2;
		_cmp_deq++;
		if (main[mid] < key)
			low = mid + 1;
		else
			high = mid;
	}
	return low;
}

void PmergeMe::pop_group_deq(std::deque<int> &dest, int pairSize)
{
	for (int i = 0; i < pairSize; i++)
	{
		dest.push_back(_deq.front());
		_deq.pop_front();
	}
}

/*
Ford-Johnson insertion step for one recursion level (deque version).

Same algorithm as the vector version, but this one consumes _deq from
the front and splits the groups into separate containers:
  first     -> b1 (the loser of the first pair)
  mainChain -> the winners (a1, a2, ...)
  pend      -> the losers waiting to be inserted (b2, b3, ... and the
               straggler if there is one)
  keys      -> the last number of each group in mainChain

1. Pop every group out of _deq into first, mainChain or pend.
2. Put b1 back at the head of mainChain with push_front. It is always
   smaller than a1, so it needs no search.
3. Insert the losers from pend in Jacobsthal order. For each loser:
     - binary search its key in keys, only before its winner's
       position (pos[t]), because a loser is always smaller than its winner
     - insert the whole group in the main chain at that position
     - add one to pos for every winner that moved one slot to the right
4. Whatever is left in _deq is the tail. Append it, then copy the main
   chain back to _deq.

The losers are read from pend by group index (t - 2), because b1 is not
in pend.
*/
void PmergeMe::insert_loosers_deq(int pairSize)
{
	int nGroups = _deq.size() / pairSize;

	if (nGroups < 2)
		return;

	int pairs = nGroups / 2;
	int nLosers = pairs + (nGroups % 2);
	std::deque<int> first;
	std::deque<int> mainChain;
	std::deque<int> keys;
	std::deque<int> pend;

	pop_group_deq(first, pairSize);
	pop_group_deq(mainChain, pairSize);
	keys.push_back(mainChain[pairSize - 1]);
	for (int k = 2; k <= pairs; k++)
	{
		pop_group_deq(pend, pairSize);
		pop_group_deq(mainChain, pairSize);
		keys.push_back(mainChain[mainChain.size() - 1]);
	}
	if (nGroups % 2 != 0)
		pop_group_deq(pend, pairSize);

	// pass 2: b1 goes at the head
	for (int i = pairSize - 1; i >= 0; i--)
		mainChain.push_front(first[i]);
	keys.push_front(first[pairSize - 1]);

	// pass 3: insert the losers in Jacobsthal order
	std::vector<int> pos(nLosers + 1);
	for (int i = 1; i <= nLosers; i++)
		pos[i] = i;

	std::vector<int> order = insertion_order(nLosers);
	for (size_t k = 0; k < order.size(); k++)
	{
		int t = order[k];
		int key = pend[(t - 1) * pairSize - 1];
		int q = binarySearch(keys, key, pos[t]);

		mainChain.insert(mainChain.begin() + q * pairSize,
						pend.begin() + (t - 2) * pairSize,
						pend.begin() + (t - 1) * pairSize);
		keys.insert(keys.begin() + q, key);
		for (int j = 1; j <= nLosers; j++)
			if (pos[j] >= q)
				pos[j]++;
	}

	// whatever is left in _deq is the sub-group tail
	while (!_deq.empty())
	{
		mainChain.push_back(_deq.front());
		_deq.pop_front();
	}

	_deq = mainChain;
}
