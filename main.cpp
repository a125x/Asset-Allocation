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
	Fund SBGD("SBGD",	15,	24.52,	6000	);	// gold from sber
	Fund SBGB("SBGB",	7.5,	12.98,	5000	);	// goverment bonds from sber
	Fund SBRB("SBRB",	2.5,	14.84,	3000	);	// corporate bonds from sber
	Fund OBLG("OBLG",	2.5,	158.1,	300	);	// corporate bonds from vim (vtb)
	Fund BOND("BOND",	2.5,	1265.,	6	);	// corporate bonds from dohod
	Fund TMOS("TMOS",	45,	7.020,	54892	);	// russian stocks from tinkoff
	Fund EQMX("EQMX",	15,	154.5,	1000	);	// russian stocks from vim
	Fund  BTC("BTC",	5,	1,	0	);	// bitcoin - i can't currently make it work so let's put it as just rubles allocated with a price of 1 rub
	Fund  XMR("XMR",	5,	1,	6482	);	// monero
	return { SBGD, SBGB, SBRB, OBLG, BOND, TMOS, EQMX, BTC, XMR };
}

// Prints fund ticker, percents in the portfolio, amount of your money in the fund and amount of fund units
// for each fund in portfolio
void printPortfolio(vector<Fund> real_portfolio)
{
	vector<vector<string>> table = {{"Ticker"}, {"Percents"}, {"Allocated"}, {"Amount"}, {"Real Percent"}};
	
	int money = 0;
	for (size_t i = 0; i < real_portfolio.size(); ++i)
		money += real_portfolio[i].getPrice() * static_cast<double>(real_portfolio[i].getAmount());

	for (size_t i = 0; i < real_portfolio.size(); ++i)
	{
		string ticker = real_portfolio[i].getTicker();
		string percent = to_string(real_portfolio[i].getPercents());
		string allocated = to_string(real_portfolio[i].getPrice() * real_portfolio[i].getAmount());
		string amount = to_string(real_portfolio[i].getAmount());
		string realPercent = to_string(real_portfolio[i].getPrice() * real_portfolio[i].getAmount() * 100.0 / money);

		table[0].push_back(ticker);
		table[1].push_back(percent);
		table[2].push_back(allocated);
		table[3].push_back(amount);
		table[4].push_back(realPercent);
	}

	// print values:
	for (size_t i = 0; i < real_portfolio.size()+1; ++i) 
	{
		for (size_t j = 0; j < table.size(); ++j) 
			cout << setw(15) << table[j][i];

		cout << '\n';
	}

	cout << endl << money << " rubles allocated" << endl << endl;
}

// Prints diff between actual and desired portfolio
void printDiff(vector<Fund> orig, vector<Fund> result)
{
	cout << setw(15) << "Ticker" << setw(15) << "Amount" << endl;

	for (size_t i = 0; i < orig.size(); ++i)
	{
		cout << setw(15) << result[i].getTicker();
		cout << setw(15) << result[i].getAmount() - orig[i].getAmount() << endl;
	}
}

// Distributes the remaining after buying funds by the data money in the same proportions to plase them
// somewhere instead of hold them in cash. This function is basically the reason why i made this programm
vector<Fund> recursiveAllocation(double money, vector<Fund> portfolio, Fund min_price)
{
	for (size_t i = 0; i < portfolio.size(); ++i)
	{
		portfolio[i].setAmount(0);
		if (portfolio[i].getPrice() * 100 / portfolio[i].getPercents() >= money)
			portfolio[i].setPercents(0);
	}

	double real_percents = 0;

	for (size_t i = 0; i < portfolio.size(); ++i)
		real_percents += portfolio[i].getPercents();

	double remaining_money = money;

	for (size_t i = 0; i < portfolio.size(); ++i)
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

	for (size_t i = 0; i < portfolio.size(); ++i)
	{
		money += portfolio[i].getPrice() * static_cast<double>(portfolio[i].getAmount());

		if (min_price.getPrice() > portfolio[i].getPrice())
			min_price = portfolio[i];
	}

	if (money >= min_price.getPrice() * 100 / min_price.getPercents())
	{
		vector<Fund> remaining_portfolio = recursiveAllocation(money, portfolio, min_price);

		for (size_t i = 0; i < portfolio.size(); ++i)
			portfolio[i].setAmount(remaining_portfolio[i].getAmount());
	}

	return portfolio;
}

int main()
{
	double money = 0;
	cin >> money;

	cout << endl << "Here's your current portfolio parameters: " << endl << endl;
	vector<Fund> orig = getPortfolio();
	printPortfolio(orig);

	cout << endl << "Here's how you can build your desired portfolio: " << endl << endl;
	vector<Fund> result = resultAllocation(money, getPortfolio());
	printPortfolio(result);

	cout << endl << "Diff (for crypto in rubles): " << endl << endl;
	printDiff(orig, result);

	return 0;
}

