#include "PmergeMe.hpp"

PmergeMe::PmergeMe()
{

}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	(void) other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	(void) other;
	return (*this);
}

PmergeMe::~PmergeMe()
{

}

bool PmergeMe::isRepeated(std::vector<int> &container, int num)
{
	std::vector<int>::const_iterator it = container.begin();
	while (it != container.end() && *it != num) ++it;
		if (!container.empty() && it != container.end())
			return (1);
	return (0);
}

void PmergeMe::fordJhonson(int argc, char **argv)
{
	int	amount = 0;
	//vector (inventa)
	std::vector<int>	vec;
	//list (with splice)
	std::list<int>		list;
	int					aux;
	long				aux2;

	for (int i = 1; i < argc - 1; i++)
	{
		aux2 = std::strtol(argv[i], NULL, 10);
		if (errno == ERANGE || aux2 > INT_MAX || aux2 < INT_MIN)
			throw ParseErrorException();
		aux = aux2;
		if (!PmergeMe::isRepeated(vec, aux))
		{
			amount++;
			vec.push_back(aux);
			list.push_back(aux);
		}
	}
	std::vector<int>::const_iterator it = vec.begin();
	std::cout << "Before:";
	while (it != vec.end())
	{
		std::cout << " " << *it;
		it++;
	}
	std::cout << std::endl;
	struct timeval begin;
	gettimeofday(&begin, 0);
	PmergeMe::recursiveVector(vec);
	PmergeMe::printTimeDiff(begin, "vector", amount);
	gettimeofday(&begin, 0);
	PmergeMe::recursiveList(list);
	PmergeMe::printTimeDiff(begin, "list", amount);
}

void PmergeMe::printTimeDiff(struct timeval &begin, std::string cont, int amount)
{
	struct timeval end;
	double result;

	gettimeofday(&end, 0);
	result = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
	std::cout << "Time to process a range of " << amount << " elements with std::"<< cont <<" : "<< result <<" us" << std::endl;
}

void PmergeMe::recursiveList(std::list<int> &list)
{
	(void) list;
	std::cout << "list" << std::endl;
}

void PmergeMe::recursiveVector(std::vector<int> &vec)
{
	std::vector<int>	main, pend;
	int					num, ind, ind_pairs;
	std::vector<int>::const_iterator it;
	t_pairs	pairs[vec.size() / 2];
	t_pairs one_pair;

	one_pair.big = 0;
	one_pair.small = 0;
	ind = 0;
	ind_pairs = 0;
	it = vec.begin();
	while (it != vec.end())
	{
		if (ind == 0)
		{
			ind++;
			num = *it;
			it++;
			continue ;
		}
		if (ind % 2 == 1)
		{
			one_pair = new t_pairs;
			pairs[ind_pairs] = one_pair;
			if (*it > num)
			{

			}
		}
		it++;
	}
	if (!vec.empty() && it != vec.end())

}

const char *PmergeMe::ParseErrorException::what() const throw()
{
	return ("Error");
}
