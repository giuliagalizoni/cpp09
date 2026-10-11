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

void PmergeMe::evaluate_pairs_vec(int pairSize)
{
	int nGroups = _vec.size() / pairSize;

	if (nGroups < 2)
		return;

	for (int i = 0; i < nGroups - 1; i += 2)
	{
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
	std::vector<int> pos(pairs + 1);
	for (int i = 1; i <= pairs; i++)
		pos[i] = i;

	std::vector<int> order = insertion_order(pairs);
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
		for (int j = 1; j <= pairs; j++)
		{
			if (pos[j] >= q)
				pos[j]++;
		}
	}

	if (nGroups % 2 != 0)
	{
		int srcGroup = nGroups - 1;
		int key = get_key_vec(srcGroup, pairSize);
		int q = binarySearch(keys, key, static_cast<int>(keys.size()));
		mainChain.insert(mainChain.begin() + q * pairSize,
				_vec.begin() + srcGroup * pairSize,
				_vec.begin() + (srcGroup + 1) * pairSize);
		keys.insert(keys.begin() + q, key);
	}

	for (size_t i = static_cast<size_t>(nGroups) * pairSize; i < _vec.size(); i++)
		mainChain.push_back(_vec[i]);

	_vec = mainChain;
}
