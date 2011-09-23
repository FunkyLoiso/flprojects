#include <string>
#include <vector>

#include <stdio.h>

using std::string;
using std::vector;

//////////////////////////////////////////////////////////////////////////
class Movie
{
public:
	enum
	{
		Childrens = 2,
		Regular = 0,
		NewRelease = 1
	};

	Movie(string title, int priceCode) : m_title(title), m_pivceCode(priceCode)
	{

	}

	int GetPivceCode() const { return m_pivceCode; }
	void SetPivceCode(int val) { m_pivceCode = val; }

	string GetTitle() const { return m_title; }

private:
	string m_title;
	int m_pivceCode;

};

//////////////////////////////////////////////////////////////////////////
class Rental
{
public:
	Rental(Movie movie, int daysRented) : m_movie(movie), m_daysRented(daysRented)
	{

	}

	Movie GetMovie() const { return m_movie; }

	int GetDaysRented() const { return m_daysRented; }

private:
	Movie m_movie;
	int m_daysRented;
};

//////////////////////////////////////////////////////////////////////////
class Customer
{
public:
	Customer(string name) : m_name(name)
	{

	}

	void AddRental(Rental arg)
	{
		m_rentals.push_back(arg);
	}

	string GetName() {return m_name;}

	string Statement()
	{
		double totalAmount = 0;
		int frequentRenterPoints = 0;
		string result = "Учёт аренды для ";
		result += GetName();
		result += "\n";
		for each(Rental each in m_rentals)
		{
			double thisAmount = 0;

			thisAmount = amountFor(each);

			//добавить очки для активного арендатора
			++frequentRenterPoints;
			//бонус за аренду новинки на два дня
			if((each.GetMovie().GetPivceCode() == Movie::NewRelease) && each.GetDaysRented() > 1)
				++frequentRenterPoints;

			//показать результат для этой аренды
			string addStr;
			addStr.resize(1024);
			sprintf(&*addStr.begin(), "\t%s\t%f\n", each.GetMovie().GetTitle().c_str(), thisAmount);

			result.append(addStr.c_str());
			totalAmount += thisAmount;
		}
		//добавим нижний колонтитул
		string addStr;
		addStr.resize(1024);
		sprintf(&*addStr.begin(), "Сумма задолженности составляет %f\nВы заработали %d очков за активность", totalAmount, frequentRenterPoints);
		result.append(addStr.c_str());

		return result;
	}

	double amountFor(const Rental& each);

private:
	string m_name;
	vector<Rental> m_rentals;
};

double Customer::amountFor(const Rental& rental)
{
	double result = 0.0;
	//определить сумму для каждой строки
	switch(rental.GetMovie().GetPivceCode())
	{
	case Movie::Regular:
		result += 2;
		if(rental.GetDaysRented() > 2)
			result += (rental.GetDaysRented() - 2) * 1.5;
		break;
	case Movie::NewRelease:
		result += rental.GetDaysRented() * 3;
		break;
	case Movie::Childrens:
		result += 1.5;
		if(rental.GetDaysRented() > 3)
			result += (rental.GetDaysRented() - 3) * 1.5;
		break;
	}	return result;
}


int main()
{
	setlocale(LC_ALL, "Russian");
	Movie M1("M1", Movie::Childrens), M2("M2", Movie::NewRelease), M3("M3", Movie::Regular), M4("M4", Movie::NewRelease);

	Rental R1(M1, 11), R2(M2, 5), R3(M3, 1), R4(M4, 2);

	Customer C1("C1");
	C1.AddRental(R1);
	C1.AddRental(R2);
	C1.AddRental(R3);
	C1.AddRental(R4);

	printf("%s", C1.Statement().c_str());

	char testResult[] = "Учёт аренды для C1\n"
		"\tM1\t13,500000\n"
		"\tM2\t15,000000\n"
		"\tM3\t2,000000\n"
		"\tM4\t6,000000\n"
		"Сумма задолженности составляет 36,500000\n"
		"Вы заработали 6 очков за активность";

	if(strcmp(C1.Statement().c_str(), testResult) == 0)
	{
		printf("\n\ntest passed");
	}
	else
	{
		printf("\n\ntest failed");
	}

	getchar();

	return 0;
}
