#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		std::cerr << "Error" << "\n";
		return (1);
	}
	for (int i = 1; i < argc; i++)
	{
		std::string s(argv[i]);
		std::string::const_iterator it = s.begin();
		while (it != s.end() && std::isdigit(*it)) ++it;
		if (!s.empty() && it != s.end())
		{
			std::cerr << "Error" << '\n';
			return (1);
		}
	}
	try
	{
		PmergeMe::fordJhonson(argc, argv);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	return (0);
}