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

	class ParseErrorException : public std::exception
	{
		virtual const char *what() const throw();
	};
};

typedef struct s_pairs
{
	int big;
	int small;
} t_pairs;