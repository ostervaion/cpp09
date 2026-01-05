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

std::vector<int> PmergeMe::getJacobNums(int size)
{
	int	newNum, prevNum, beforePrev;
	std::vector<int> secuence, edit;
	std::vector<int>::const_iterator it;

	prevNum = 1;
	beforePrev = 1;
	newNum = 0;
	while (1)
	{
		newNum = prevNum + 2 * beforePrev;
		if (newNum > size)
			break ;
		secuence.push_back(newNum);
		beforePrev = prevNum;
		prevNum = newNum;
	}
	it = secuence.begin();
	//use beforePrev as an int holder
	while (it != secuence.end())
	{
		beforePrev = *it;
		while (beforePrev > 1 && !isRepeated(edit, beforePrev))
		{
			edit.push_back(beforePrev);
			beforePrev--;
		}
		it++;
	}
	if (edit.size() != size)
	{
		beforePrev = size;
		while (beforePrev > 1 && !isRepeated(edit, beforePrev))
		{
			edit.push_back(beforePrev);
			beforePrev--;
		}
	}
	return (edit);
}

//main is the container where the chain is going to be inserted in order
//pend are the numbers to insert
//edit is the container with the jacobsthal numbers that tell us the numbers that must be inserted first
//pairs shows the pairs that where made before to know the limit of the comparisons
void sortPendIntoMain(std::vector<int> &main,std::vector<int> &pend, std::vector<int> &edit, t_pairs *pairs)
{
	std::vector<int>::const_iterator it, mainIt;
	int ind, auxInd, pairAmount;
	
	pairAmount = main.size();
	main.insert(main.begin(), pend.at(0));
	it = edit.begin();
	//comprobar los valores añadidos sin pareja
	while (it != edit.end())
	{
		ind = 0;
		mainIt = main.begin();
		ind = 0;
		for(mainIt = main.begin(); mainIt != main.end() && (pairAmount < *it || *mainIt != pairs[*it - 1].big); mainIt++) ind++;
		auxInd = 0;
		while (1)
		{
			//binary search
			if (pairs[*it - 1].small > main.at(ind) && pairs[*it - 1].small < main.at(auxInd))
			{
				if (ind - auxInd <= 1)
					break ;
			}
			if (main.at((auxInd + ind) / 2) < pairs[*it - 1].small)
			{
				//rigth
				ind = (auxInd + ind) / 2;
			}
			else
			{
				//left
				auxInd = (auxInd + ind) / 2;
			}
		}
		main.insert(main.begin() + auxInd, pend.at(*it - 1));
		it++;
	}
}

void PmergeMe::recursiveVector(std::vector<int> &vec)
{
	std::vector<int>	main, pend;
	int					num, ind;
	std::vector<int>::const_iterator it;
	t_pairs	pairs[vec.size() / 2];

	ind = 0;
	it = vec.begin();
	while (it != vec.end())
	{
		if (ind % 2 == 0)
			num = *it;
		else
		{
			if (*it > num)
			{
				pairs[ind/2].big = *it;
				pairs[ind/2].small = num;
			}
			else
			{
				pairs[ind/2].big = num;
				pairs[ind/2].small = *it;
			}
		}
		ind++;
		it++;
	}
	for (int i = 0; i < vec.size() / 2 ; i++)
	{
		main.push_back(pairs[i].big);
		pend.push_back(pairs[i].small);
	}
	if (ind % 2 == 1)
		pend.push_back(num);
	if (int i = vec.size(); i != 2 && i != 3)
		recursiveVector(main);
	//apply jacobsthal
	std::vector<int> edit;
	edit = getJacobNums(pend.size());
	sortPendIntoMain(main, pend, edit, pairs);
	vec = main;
}

const char *PmergeMe::ParseErrorException::what() const throw()
{
	return ("Error");
}
