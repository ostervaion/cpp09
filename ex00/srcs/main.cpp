#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	std::ifstream inputf(argv[1]);
	if (!inputf.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	try
	{
		BitcoinExchange btc;
		btc.processInputFile(inputf);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		inputf.close();
		return 1;
	}

	inputf.close();
	return (0);
}