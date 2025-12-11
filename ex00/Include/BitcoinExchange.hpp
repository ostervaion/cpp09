#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <cstdlib>
#include <exception>

class BitcoinExchange
{
private:
	std::map<std::string, double>	_database;
	bool isLeap(int year) const;
	bool isValidDate(int d, int m, int y) const;
	bool parseDate(const std::string &date, int &year, int &month, int &day) const;
	void loadDatabase(const std::string &filename);
	double getExchangeRate(const std::string &date) const;
	void processLine(const std::string &line);

public:
	BitcoinExchange();
	BitcoinExchange(const std::string &databaseFile);
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	void processInputFile(std::ifstream &file);

	class FileCannotBeOpened : public std::exception
	{
		virtual const char *what() const throw();
	};

	class InvalidDatabase : public std::exception
	{
		virtual const char *what() const throw();
	};
};