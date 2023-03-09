// This programm was created in goal to helps me to distribute my own portfolio
// which consists ftom funds only
// However, it can be used in case of stocks distribution
// 
// You have to add your funds/stocks right in the 'vector<Fund> getPortfolio()' function
// The format is 
// { "Ticker", percentage, current price per stock, current quantity of stocks in your portfolio }
// Currently this function have my own portfolio distribution
// After this start the programm and enter the amount of money you want to add

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

// A simple class contains main fund parametets
class Fund
{
public:
	const string getTicker() { return this->ticker; }
	const double getPercents() { return this->percents; }
	const double getPrice() { return this->price; }
	const int getAmount() { return this->amount; }
	void setTicker(const string ticker) { this->ticker = ticker; }
	void setPercents(const double percents) { this->percents = percents; }
	void setPrice(const double price) { this->price = price; }
	void setAmount(const int amount) { this->amount = amount; }

	Fund(const string ticker, const double percents, const double price, const int amount)
	{
		this->ticker = ticker;
		this->percents = percents;
		this->price = price;
		this->amount = amount;
	}

private:
	string ticker;
	double percents;
	double price;
	int amount;
};

// Put your portfolio data into this function
//("TICKER", percentage, price, amount of shares)
vector<Fund> getPortfolio()
{
	Fund GOLD("GOLD", 10, 1.1606, 100); // gold from vim (vtb)
	Fund SBGB("SBGB", 10, 12.315, 10); // goverment bonds from sber
	Fund SBRB("SRGB", 5, 12.739, 10); // corporate bonds from sber
	Fund OBLG("OBLG", 5, 134.7, 1); // corporate bonds from vim (vtb)
	Fund TMOS("TMOS", 70, 4.382, 3500); // russian stocks from tinkoff
	return { GOLD, SBGB, SBRB, OBLG, TMOS };
}

// Prints fund ticker, percents in the portfolio, amount of your money in the fund and amount of fund units
// for each fund in portfolio
void printPortfolio(vector<Fund> real_portfolio)
{
	cout.setf(ios::fixed);
	cout <<setw(10)<< "Ticker" << "Percents" << "Overall money" << "Amount" << endl;
	int money = 0;
	for (int i = 0; i < real_portfolio.size(); ++i)
		money += real_portfolio[i].getPrice() * static_cast<double>(real_portfolio[i].getAmount());
	for (int i = 0; i < real_portfolio.size(); ++i)
	{
		cout << real_portfolio[i].getTicker() << ' '
			<< real_portfolio[i].getPercents() << ' '
			<< real_portfolio[i].getPrice() * real_portfolio[i].getAmount() << ' '
			<< real_portfolio[i].getAmount() << ' '
			<< real_portfolio[i].getPrice() * real_portfolio[i].getAmount() * 100.0 / money << endl;
	}
	cout << money << endl;
}

// Distributes the remaining after buying funds by the data money in the same proportions to plase them
// somewhere instead of hold them in cash. This function is basically the reason why i made this programm
vector<Fund> recursiveAllocation(double money, vector<Fund> portfolio, Fund min_price)
{
	for (int i = 0; i < portfolio.size(); ++i)
	{
		portfolio[i].setAmount(0);
		if (portfolio[i].getPrice() * 100 / portfolio[i].getPercents() >= money)
			portfolio[i].setPercents(0);
	}

	double real_percents = 0;

	for (int i = 0; i < portfolio.size(); ++i)
		real_percents += portfolio[i].getPercents();

	double remaining_money = money;

	for (int i = 0; i < portfolio.size(); ++i)
	{
		portfolio[i].setPercents(portfolio[i].getPercents() * (100 / real_percents));
		portfolio[i].setAmount((portfolio[i].getPercents() * money / 100) / portfolio[i].getPrice());
		remaining_money -= portfolio[i].getPrice() * portfolio[i].getAmount();
	}

	if (remaining_money > min_price.getPrice() * 100 / min_price.getPercents())
	{
		vector<Fund> remaining_portfolio = recursiveAllocation(remaining_money, portfolio, min_price);

		for (int i = 0; i < portfolio.size(); ++i)
			portfolio[i].setAmount(portfolio[i].getAmount() + remaining_portfolio[i].getAmount());
	}

	return portfolio;
}

// First distribution and recurcive function calling, returns result portfolio
vector<Fund> resultAllocation(double money, vector<Fund> portfolio)
{
	Fund min_price = portfolio[0];

	for (int i = 0; i < portfolio.size(); ++i)
	{
		money += portfolio[i].getPrice() * static_cast<double>(portfolio[i].getAmount());

		if (min_price.getPrice() > portfolio[i].getPrice())
			min_price = portfolio[i];
	}

	if (money >= min_price.getPrice() * 100 / min_price.getPercents())
	{
		vector<Fund> remaining_portfolio = recursiveAllocation(money, portfolio, min_price);

		for (int i = 0; i < portfolio.size(); ++i)
			portfolio[i].setAmount(remaining_portfolio[i].getAmount());
	}

	return portfolio;
}

int main()
{
	double money = 0;
	cin >> money;
	printPortfolio(resultAllocation(money, getPortfolio()));
	return 0;
}

