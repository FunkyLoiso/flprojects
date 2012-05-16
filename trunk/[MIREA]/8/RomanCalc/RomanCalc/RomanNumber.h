#pragma once

#include <QString>

class RomanNumber
{
public:

	static RomanNumber FromInt(int val);
	static RomanNumber FromString(QString str);

	RomanNumber();
	RomanNumber(int);
	RomanNumber(const QString&);
	RomanNumber(const RomanNumber&);

	~RomanNumber(void);

	bool isValid();
	QString toString();
	int toInt();

	RomanNumber operator+(const RomanNumber&);
	RomanNumber operator-(const RomanNumber&);
	RomanNumber operator*(const RomanNumber&);
	RomanNumber& operator=(const RomanNumber&);
	bool operator==(const RomanNumber&);
	RomanNumber& operator+=(const RomanNumber&);
	RomanNumber& operator-=(const RomanNumber&);
	RomanNumber& operator*=(const RomanNumber&);

private:
	static inline int value(QChar ch);
	static int evaluatePositionString( QString &str);

	int m_val;
	bool m_isValid;

	inline bool chech(int val) {return val > -4000 && val < 4000;}
};
