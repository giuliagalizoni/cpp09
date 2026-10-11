#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <iostream>
# include <string>
# include <vector>
# include <deque>
# include <algorithm>
# include <stdexcept>
# include <climits>
# include <cctype>
# include <ctime>

class PmergeMe
{
	private:
		std::vector<int> _vec;
		std::deque<int> _deq;
		double _time_vec;
		double _time_deq;

		// PmergeMeVector.cpp
		int get_key_vec(int i, int pairSize) const;
		void swap_groups_vec(int iA, int iB, int pairSize);
		void evaluate_pairs_vec(int pairSize); // recursive func
		int binarySearch(const std::vector<int> &main, int key, int bound) const;
		void append_group_vec(std::vector<int> &dest, int groupIndex, int pairSize) const;
		void insert_loosers_vec(int pairSize);

		// PmergeMeDeque.cpp
		int get_key_deq(int i, int pairSize) const;
		void swap_groups_deq(int iA, int iB, int pairSize);
		void evaluate_pairs_deq(int pairSize); // recursive func
		int binarySearch(const std::deque<int> &main, int key, int bound) const;
		void pop_group_deq(std::deque<int> &dest, int pairSize);
		void insert_loosers_deq(int pairSize);

		// PmergeMeUtils.cpp
		std::vector<int> jacobsthal(int n) const;
		std::vector<int> insertion_order(int nPairs) const;
		bool validate(const std::string &arg, int &value) const;

	public:
		// PmergeMe.cpp
		PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe& operator=(const PmergeMe& other);
		~PmergeMe();

		void parseArgs(int count, char** args);
		void sort_vec();
		void sort_deq();

		const std::vector<int>& getVec() const;
		const std::deque<int>&  getDeq() const;
		double getVecTime() const;
		double getDeqTime() const;
};

#endif
