#include "BitcoinExchange.hpp"

// Returns true if year is a leap year
bool BitcoinExchange::isLeap(int year) const
{
	return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

// Validates if a date is valid
bool BitcoinExchange::isValidDate(int d, int m, int y) const
{
	if (y < 1 || m < 1 || m > 12 || d < 1 || d > 31)
		return false;

	// February
	if (m == 2)
	{
		if (isLeap(y))
			return (d <= 29);
		return (d <= 28);
	}

	// April, June, September, November have 30 days
	if (m == 4 || m == 6 || m == 9 || m == 11)
		return (d <= 30);

	return true;
}

// Parse a date string "YYYY-MM-DD" into components
bool BitcoinExchange::parseDate(const std::string &date, int &year, int &month, int &day) const
{
	if (date.length() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;

	// Check all characters are digits except dashes
	for (size_t i = 0; i < date.length(); i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(date[i]))
			return false;
	}

	year = std::atoi(date.substr(0, 4).c_str());
	month = std::atoi(date.substr(5, 2).c_str());
	day = std::atoi(date.substr(8, 2).c_str());

	return isValidDate(day, month, year);
}

// Load the database CSV file
void BitcoinExchange::loadDatabase(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw FileCannotBeOpened();

	std::string line;
	bool firstLine = true;

	while (std::getline(file, line))
	{
		// Skip header line
		if (firstLine)
		{
			firstLine = false;
			continue;
		}

		// Find comma separator
		size_t commaPos = line.find(',');
		if (commaPos == std::string::npos)
			continue;

		std::string date = line.substr(0, commaPos);
		std::string rateStr = line.substr(commaPos + 1);

		// Validate date format
		int year, month, day;
		if (!parseDate(date, year, month, day))
			continue;

		// Parse rate
		double rate = std::atof(rateStr.c_str());
		_database[date] = rate;
	}

	file.close();

	if (_database.empty())
		throw InvalidDatabase();
}

// Get exchange rate for a date (or closest earlier date)
double BitcoinExchange::getExchangeRate(const std::string &date) const
{
	// Try exact match first
	std::map<std::string, double>::const_iterator it = _database.find(date);
	if (it != _database.end())
		return it->second;

	// Find the closest earlier date using upper_bound
	it = _database.upper_bound(date);
	if (it == _database.begin())
		return -1; // Date is before any in database

	--it; // Move to the previous entry (closest date <= requested date)
	return it->second;
}

// Process a single line from input file
void BitcoinExchange::processLine(const std::string &line)
{
	// Skip empty lines
	if (line.empty())
		return;

	// Find separator " | "
	size_t sepPos = line.find(" | ");
	if (sepPos == std::string::npos)
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return;
	}

	std::string date = line.substr(0, sepPos);
	std::string valueStr = line.substr(sepPos + 3);

	// Validate date
	int year, month, day;
	if (!parseDate(date, year, month, day))
	{
		std::cerr << "Error: bad input => " << date << std::endl;
		return;
	}

	// Parse and validate value
	char *endptr;
	double value = std::strtod(valueStr.c_str(), &endptr);

	// Check if conversion was successful
	if (*endptr != '\0' && *endptr != '\n' && *endptr != '\r')
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return;
	}

	if (value < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return;
	}

	if (value > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return;
	}

	// Get exchange rate
	double rate = getExchangeRate(date);
	if (rate < 0)
	{
		std::cerr << "Error: date is before database records." << std::endl;
		return;
	}

	// Calculate and output result
	double result = value * rate;
	std::cout << date << " => " << value << " = " << result << std::endl;
}

// Default constructor - loads database from default path
BitcoinExchange::BitcoinExchange()
{
	loadDatabase("data.csv");
}

// Constructor with custom database path
BitcoinExchange::BitcoinExchange(const std::string &databaseFile)
{
	loadDatabase(databaseFile);
}

// Copy constructor
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	_database = other._database;
}

// Assignment operator
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_database = other._database;
	return *this;
}

// Destructor
BitcoinExchange::~BitcoinExchange()
{
}

// Process the entire input file
void BitcoinExchange::processInputFile(std::ifstream &file)
{
	std::string line;
	bool firstLine = true;

	while (std::getline(file, line))
	{
		// Skip header line if present
		if (firstLine)
		{
			firstLine = false;
			if (line == "date | value")
				continue;
		}
		processLine(line);
	}
}

const char *BitcoinExchange::FileCannotBeOpened::what() const throw()
{
	return "Error: could not open file.";
}

const char *BitcoinExchange::InvalidDatabase::what() const throw()
{
	return "Error: database is empty or invalid.";
}
