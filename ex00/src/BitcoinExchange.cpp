#include "../includes/BitcoinExchange.hpp"

# include <cstdlib>
# include <cctype>
# include <cmath>

// Default constructor
BitcoinExchange::BitcoinExchange() {}

// Copy constructor
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _db(other._db) {}
// Copy assignment operator
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
	{
		_db = other._db;
	}
	return *this;
}

// Destructor
BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDatabase(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open database");

	std::string line;
	std::getline(file, line);
	while (std::getline(file, line))
	{
		size_t pos = line.find(',');
		if (pos == std::string::npos)
		{
			std::cerr << "Error: invalid line in database" << std::endl;
			continue;
		}
		std::string date = trim(line.substr(0, pos));
		std::string rateStr = trim(line.substr(pos + 1));
		char *endptr;
		double rate = strtod(rateStr.c_str(), &endptr);
		if (endptr == rateStr.c_str() || *endptr != '\0')
		{
			std::cerr << "Error: invalid rate in database" << std::endl;
			continue;
		}
		if (std::isinf(rate) || std::isnan(rate) || rate < 0)
		{
			std::cerr << "Error: invalid rate in database" << std::endl;
			continue;
		}
		if (!isValidFormat(date)
			|| !isValidRange(std::atoi(date.substr(0, 4).c_str()),
				std::atoi(date.substr(5, 2).c_str()),
				std::atoi(date.substr(8, 2).c_str())))
		{
			std::cerr << "Error: invalid date in database" << std::endl;
			continue;
		}
		_db[date] = rate;
	}
	file.close();
}

void BitcoinExchange::processInput(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open input file");

	std::string line;
	while (std::getline(file, line))
	{
		if (trim(line) == "date | value")
			continue;
		size_t pos = line.find('|');
		if (pos == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		std::string date = trim(line.substr(0, pos));
		std::string value = trim(line.substr(pos + 1));
		if (!validateDate(date))
			continue;
		double amount;
		if (!validateValue(value, amount))
			continue;
		double rate = getRate(date);
		if (rate < 0)
			continue;
		std::cout << date << " => " << value << " = " << (amount * rate)<< std::endl;
	}
	file.close();
}

std::string BitcoinExchange::trim(const std::string& str)
{
	size_t start = str.find_first_not_of(" \t\r\n");
	size_t end = str.find_last_not_of(" \t\r\n");

	if (start == std::string::npos)
		return "";
	return str.substr(start, end - start + 1);
}

bool BitcoinExchange::validateDate(const std::string& date)
{

	if (!isValidFormat(date))
	{
		std::cerr << "Error: bad input => " << date << std::endl;
		return false;
	}

	int year  = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day   = std::atoi(date.substr(8, 2).c_str());

	if (!isValidRange(year, month, day))
	{
		std::cerr << "Error: bad input => " << date << std::endl;
		return false;
	}
	return true;
}

bool BitcoinExchange::isValidFormat(const std::string& date)
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	for (size_t i = 0; i < date.size(); i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!isdigit(date[i]))
			return false;
	}
	return true;
}

bool BitcoinExchange::isValidRange(int year, int month, int day)
{
	if (month < 1 || month > 12)
		return false;

	int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (isLeapYear(year))
		days_in_month[1] = 29;

	if (day < 1 || day > days_in_month[month - 1])
		return false;
	return true;
}

bool BitcoinExchange::isLeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool BitcoinExchange::validateValue(const std::string &value, double &amount)
{
	char *endptr;
	const char *start = value.c_str();
	amount = strtod(start, &endptr);
	if (endptr == start || *endptr != '\0')
	{
		std::cerr << "Error: bad input =>" << value << std::endl;
		return false;
	}

	if (std::isinf(amount) || std::isnan(amount))
	{
		std::cerr << "Error: bad input => " << value << std::endl;
		return false;
	}

	if (amount < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}

	if (amount > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return false;
	}

	return true;
}

double BitcoinExchange::getRate(const std::string& date)
{
	std::map<std::string, double>::iterator it = _db.lower_bound(date);

	if (it != _db.end() && it->first == date)
		return it->second;

	if (it == _db.begin())
	{
		std::cerr << "Error: no data for date => " << date << std::endl;
		return -1;
	}

	--it;
	return it->second;
}
