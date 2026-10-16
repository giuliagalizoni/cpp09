#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <string>
# include <map>
# include <fstream>
# include <iostream>
# include <stdexcept>

class BitcoinExchange
{
	private:
		std::map<std::string, double> _db;

		bool validateDate(const std::string &date);
		bool validateValue(const std::string &value, double &amount);
		std::string trim(const std::string &str);
		bool isValidFormat(const std::string &date);
		bool isLeapYear(int year);
		bool isValidRange(int year, int month, int day);
		double getRate(const std::string& date);

	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();

		void loadDatabase(const std::string& filename);
		void processInput(const std::string& filename);
};

#endif
