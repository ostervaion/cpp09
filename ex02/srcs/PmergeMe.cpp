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

bool PmergeMe::isRepeated(std::list<int> &container, int num)
{
	std::list<int>::const_iterator it = container.begin();
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

	for (int i = 1; i < argc; i++)
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
		else
			std::cout << "Number " << aux << " skipped because it is already in the list" << std::endl;
	}
	std::cout << "Before: ";
	PmergeMe::printNums(vec);
	struct timeval begin;
	gettimeofday(&begin, 0);
	PmergeMe::recursiveVector(vec);
	//PmergeMe::isSorted(vec);
	std::cout << "After: ";
	PmergeMe::printNums(vec);
	PmergeMe::printTimeDiff(begin, "vector", amount);
	gettimeofday(&begin, 0);
	PmergeMe::recursiveList(list);
	//PmergeMe::isSorted(list);
	PmergeMe::printTimeDiff(begin, "list", amount);
}

void PmergeMe::printNums(const std::vector<int> &container)
{
	std::vector<int>::const_iterator it2 = container.begin();
	while (it2 != container.end())
	{
		std::cout << " " << *it2;
		it2++;
	}
	std::cout << std::endl;
}

void PmergeMe::printNums(const std::list<int> &container)
{
	std::list<int>::const_iterator it2 = container.begin();
	while (it2 != container.end())
	{
		std::cout << " " << *it2;
		it2++;
	}
	std::cout << std::endl;
}

void PmergeMe::printTimeDiff(struct timeval &begin, std::string cont, int amount)
{
	struct timeval end;
	double result;

	gettimeofday(&end, 0);
	result = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
	std::cout << std::fixed <<"Time to process a range of " << amount << " elements with std::"<< cont <<" : "<< result / 1000000 <<" us" << std::endl;
}

void PmergeMe::recursiveList(std::list<int> &list)
{
	std::list<int>	main, pend;
	int					num, ind;
	std::list<int>::const_iterator it;
	t_pairs	pairs[list.size() / 2];

	ind = 0;
	it = list.begin();
	while (it != list.end())
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
	for (int i = 0; i < static_cast<int>(list.size()) / 2 ; i++)
	{
		main.push_back(pairs[i].big);
		pend.push_back(pairs[i].small);
	}
	if (ind % 2 == 1)
		pend.push_back(num);
	int i = list.size();
	if (i != 2 && i != 3)
		recursiveList(main);
	//apply jacobsthal
	std::list<int> edit;
	edit = getJacobNumsList(pend.size());
	sortPend(main, pend, pairs);
	sortPendIntoMain(main, pend, edit, pairs);
	list = main;
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
	if (static_cast<int>(edit.size()) != size)
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

void PmergeMe::sortPend(const std::vector<int> &main, std::vector<int> &pend, t_pairs *pairs)
{
	int i, j;
	std::vector<int> newPend;
	t_pairs newPairs[main.size()];

	j = 0;
	for(std::vector<int>::const_iterator it = main.begin(); it != main.end(); it++)
	{
		i = 0;
		while (pairs[i].big != *it)
			i++;
		newPend.push_back(pairs[i].small);
		newPairs[j].big = pairs[i].big;
		newPairs[j].small = pairs[i].small;
		j++;
	}
	if (pend.size() != newPend.size())
		newPend.push_back(pend.back());
	pend = newPend;
	for (size_t j = 0; j != main.size(); j++)
		pairs[j] = newPairs[j];
}


//main is the container where the chain is going to be inserted in order
//pend are the numbers to insert
//edit is the container with the jacobsthal numbers that tell us the numbers that must be inserted first
//pairs shows the pairs that where made before to know the limit of the comparisons
void PmergeMe::sortPendIntoMain(std::vector<int> &main,std::vector<int> &pend, std::vector<int> &edit, t_pairs *pairs)
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
		//comprobar la comparación de pairAmount e *it
		if (pairAmount < *it)
			ind = main.size() - 1;
		else
		{
			for(mainIt = main.begin(); mainIt != main.end() && *mainIt != pairs[*it - 1].big; mainIt++)
			{
				ind++;
			}
		}
		auxInd = 0;
		while (1)
		{
				if (pend.at(*it - 1) < main.at(ind) && pend.at(*it - 1) > main.at(auxInd))
				{
					if (ind - auxInd <= 1)
						break ;
				}
				else if(pend.at(*it - 1) > main.at(ind))
				{
					ind++;
					break ;
				}
				else if(pend.at(*it - 1) < main.at(auxInd))
				{
					ind = auxInd;
					break ;
				}
				if (main.at((auxInd + ind) / 2) < pend.at(*it - 1))
				{
					//rigth
					auxInd = (auxInd + ind) / 2;
				}
				else
				{
					//left
					ind = (auxInd + ind) / 2;
				}
		}
		main.insert(main.begin() + ind, pend.at(*it - 1));
		it++;
	}
}

void PmergeMe::isSorted(const std::vector<int> &vec)
{
	int	i, j;
	std::vector<int>::const_iterator it = vec.begin();

	i = *it;
	it++;
	while(it != vec.end())
	{
		j = *it;
		if (i > j)
		{
			std::cout << "Not sorted" << std::endl;
			return ;
		} 
		i = j;
		it++;
	}
	std::cout << "Sorted" << std::endl;
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
	for (int i = 0; i < static_cast<int>(vec.size()) / 2 ; i++)
	{
		main.push_back(pairs[i].big);
		pend.push_back(pairs[i].small);
	}
	if (ind % 2 == 1)
		pend.push_back(num);
	int i = vec.size();
	if (i != 2 && i != 3)
		recursiveVector(main);
	//apply jacobsthal
	std::vector<int> edit;
	edit = getJacobNums(pend.size());
	sortPend(main, pend, pairs);
	sortPendIntoMain(main, pend, edit, pairs);
	vec = main;
}

const char *PmergeMe::ParseErrorException::what() const throw()
{
	return ("Error");
}

std::list<int> PmergeMe::getJacobNumsList(int size)
{
	int	newNum, prevNum, beforePrev;
	std::list<int> secuence, edit;
	std::list<int>::const_iterator it;

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
	if (static_cast<int>(edit.size()) != size)
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

int PmergeMe::getFromList(const std::list<int> &list, const int &ind)
{
	std::list<int>::const_iterator it = list.begin();
	for (int i = 0; ind != i; i++)
		it++;
	return (*it);
}

void PmergeMe::sortPendIntoMain(std::list<int> &main,std::list<int> &pend, std::list<int> &edit, t_pairs *pairs)
{
	std::list<int>::const_iterator it, mainIt;
	int ind, auxInd, pairAmount;

	pairAmount = main.size();
	main.insert(main.begin(), getFromList(pend, 0));
	it = edit.begin();
	//comprobar los valores añadidos sin pareja
	while (it != edit.end())
	{
		ind = 0;
		mainIt = main.begin();
		ind = 0;
		//comprobar la comparación de pairAmount e *it
		if (pairAmount < *it)
			ind = main.size() - 1;
		else
		{
			for(mainIt = main.begin(); mainIt != main.end() && *mainIt != pairs[*it - 1].big; mainIt++)
			{
				ind++;
			}
		}
		auxInd = 0;
		while (1)
		{
				if (getFromList(pend, *it - 1) < getFromList(main, ind) && getFromList(pend, *it - 1) > getFromList(main, auxInd))
				{
					if (ind - auxInd <= 1)
						break ;
				}
				else if(getFromList(pend, *it - 1) > getFromList(main, ind))
				{
					ind++;
					break ;
				}
				else if(getFromList(pend, *it - 1) < getFromList(main, auxInd))
				{
					ind = auxInd;
					break ;
				}
				if (getFromList(main, (auxInd + ind) / 2) < getFromList(pend, *it - 1))
				{
					//rigth
					auxInd = (auxInd + ind) / 2;
				}
				else
				{
					//left
					ind = (auxInd + ind) / 2;
				}
		}
		std::list<int>::iterator it2 = main.begin();
		for (int j = 0; j != ind; j++)
			it2++;
		main.insert(it2, getFromList(pend, *it - 1));
		it++;
	}
}

void PmergeMe::sortPend(const std::list<int> &main, std::list<int> &pend, t_pairs *pairs)
{
	int i, j;
	std::list<int> newPend;
	t_pairs newPairs[main.size()];

	j = 0;
	for(std::list<int>::const_iterator it = main.begin(); it != main.end(); it++)
	{
		i = 0;
		while (pairs[i].big != *it)
			i++;
		newPend.push_back(pairs[i].small);
		newPairs[j].big = pairs[i].big;
		newPairs[j].small = pairs[i].small;
		j++;
	}
	if (pend.size() != newPend.size())
		newPend.push_back(pend.back());
	pend = newPend;
	for (size_t j = 0; j != main.size(); j++)
		pairs[j] = newPairs[j];
}

void PmergeMe::isSorted(const std::list<int> &vec)
{
	int	i, j;
	std::list<int>::const_iterator it = vec.begin();

	i = *it;
	it++;
	while(it != vec.end())
	{
		j = *it;
		if (i > j)
		{
			std::cout << "Not sorted" << std::endl;
			return ;
		} 
		i = j;
		it++;
	}
	std::cout << "Sorted" << std::endl;
}