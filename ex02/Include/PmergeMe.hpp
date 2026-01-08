#pragma once

#include <errno.h>
#include <iostream>
#include <exception>
#include <vector>
#include <list>
#include <limits.h>
#include <cstdlib>
#include <sys/time.h>
#include <stdio.h>

typedef struct s_pairs
{
	int big;
	int small;
} t_pairs;

class PmergeMe
{
	private:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();
	public:
		static bool isRepeated(std::vector<int> &container, int num);
		static void recursiveVector(std::vector<int> &vec);
		static void recursiveList(std::list<int> &list);
		static void printTimeDiff(struct timeval &begin, std::string cont, int amount);
		static void fordJhonson(int argc, char **argv);
		static std::vector<int>getJacobNums(int size);
		static void sortPendIntoMain(std::vector<int> &main,std::vector<int> &pend, std::vector<int> &edit, t_pairs *pairs);
		static void sortPend(const std::vector<int> &main, std::vector<int> &pend, t_pairs *pairs);
		static void isSorted(const std::vector<int> &vec);
		static std::list<int>getJacobNumsList(int size);
		static void printNums(const std::vector<int> &container);
		static void printNums(const std::list<int> &container);
		static bool isRepeated(std::list<int> &container, int num);
		static void sortPendIntoMain(std::list<int> &main,std::list<int> &pend, std::list<int> &edit, t_pairs *pairs);
		static void sortPend(const std::list<int> &main, std::list<int> &pend, t_pairs *pairs);
		static void isSorted(const std::list<int> &vec);
		static int getFromList(const std::list<int> &list, const int &ind);
	class ParseErrorException : public std::exception
	{
		virtual const char *what() const throw();
	};
};