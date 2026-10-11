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

void PmergeMe::evaluate_pairs_deq(int pairSize)
{
	int nGroups = _deq.size() / pairSize;

	if (nGroups < 2)
		return;

	for (int i = 0; i < nGroups - 1; i += 2)
	{
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

void PmergeMe::insert_loosers_deq(int pairSize)
{
	int nGroups = _deq.size() / pairSize;

	if (nGroups < 2)
		return;

	int pairs = nGroups / 2;
	std::deque<int> first;
	std::deque<int> mainChain;
	std::deque<int> keys;
	std::deque<int> pend;
	std::deque<int> straggler;

	// pass 1: consume _deq from the front, in container order
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
		pop_group_deq(straggler, pairSize);

	// pass 2: b1 goes at the head
	for (int i = pairSize - 1; i >= 0; i--)
		mainChain.push_front(first[i]);
	keys.push_front(first[pairSize - 1]);

	// pass 3: insert the losers in Jacobsthal order
	std::vector<int> pos(pairs + 1);
	for (int i = 1; i <= pairs; i++)
		pos[i] = i;

	std::vector<int> order = insertion_order(pairs);
	for (size_t k = 0; k < order.size(); k++)
	{
		int t = order[k];
		int key = pend[(t - 1) * pairSize - 1];
		int q = binarySearch(keys, key, pos[t]);

		mainChain.insert(mainChain.begin() + q * pairSize,
		                 pend.begin() + (t - 2) * pairSize,
		                 pend.begin() + (t - 1) * pairSize);
		keys.insert(keys.begin() + q, key);
		for (int j = 1; j <= pairs; j++)
			if (pos[j] >= q)
				pos[j]++;
	}

	if (!straggler.empty())
	{
		int key = straggler[pairSize - 1];
		int q = binarySearch(keys, key, static_cast<int>(keys.size()));

		mainChain.insert(mainChain.begin() + q * pairSize, straggler.begin(), straggler.end());
		keys.insert(keys.begin() + q, key);
	}

	// whatever is left in _deq is the sub-group tail
	while (!_deq.empty())
	{
		mainChain.push_back(_deq.front());
		_deq.pop_front();
	}

	_deq = mainChain;
}
