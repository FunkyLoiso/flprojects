#include "RomanNumber.h"
#include <QRegexp.h>

RomanNumber::RomanNumber()
{
	m_isValid = false;

}

RomanNumber::RomanNumber( int val )
{
	m_isValid = true;
	if(!chech(val)) m_isValid = false;
	m_val = val;
}

RomanNumber::RomanNumber( const QString& inStr )
{
	m_isValid = false;
	if(inStr.isEmpty()) return;
	m_val = 0;
	QString str = inStr.simplified();
	str.remove(' ');
	str = str.toUpper();

	bool neg = str[0] == '-';

	int value = 0;

	int hundredsIndex = str.indexOf(QRegExp("[DCLXVI]"));
	QString thousandas = str.left(hundredsIndex);
	int tensIndex = str.indexOf(QRegExp("[LXVI]"), hundredsIndex);
	QString hundreds = str.mid(hundredsIndex, tensIndex - hundredsIndex);
	int onesIndex = str.indexOf(QRegExp("[VI]"), tensIndex);
	QString tens = str.mid(tensIndex, onesIndex - tensIndex);
	QString ones = onesIndex > -1 ? str.mid(onesIndex) : QString();

	value = evaluatePositionString(thousandas);
	if(value < 0 || value > 3000) return;
	m_val += value;
	value = evaluatePositionString(hundreds);
	if(value < 0 || value > 900) return;
	m_val += value;
	value = evaluatePositionString(tens);
	if(value < 0 || value > 90) return;
	m_val += value;
	value = evaluatePositionString(ones);
	if(value < 0 || value > 9) return;
	m_val += value;

	if(neg) m_val = -m_val;
	m_isValid = true;
}

RomanNumber::RomanNumber( const RomanNumber& other )
{
	*this = other;
}

RomanNumber::~RomanNumber(void)
{
}

bool RomanNumber::isValid()
{
	return m_isValid;
}

QString RomanNumber::toString()
{
	if(!m_isValid) return QString(" #invalid# ");

	QString res;

	int val = qAbs(m_val);
	for(; val >= 1000; val -= 1000) res.append('M');
	if(val >= 900)
	{
		res.append("CM");
		val -= 900;
	}
	if(val >= 500)
	{
		res.append('D');
		val -= 500;
	}
	if(val >= 400)
	{
		res.append("CD");
		val -= 400;
	}
	for(; val >= 100; val -= 100) res.append('C');

	if(val >= 90)
	{
		res.append("XC");
		val -= 90;
	}
	if(val >= 50)
	{
		res.append('L');
		val -= 50;
	}
	if(val >= 40)
	{
		res.append("XL");
		val -= 40;
	}
	for(; val >= 10; val -= 10) res.append('X');

	if(val >= 9)
	{
		res.append("IX");
		val -= 9;
	}
	if(val >= 5)
	{
		res.append('V');
		val -= 5;
	}
	if(val >= 4)
	{
		res.append("IV");
		val -= 4;
	}
	for(; val > 0; val -= 1) res.append('I');

	if(m_val < 0) res.prepend('-');
	return res;
}

int RomanNumber::toInt()
{
	if(m_isValid) return m_val;
	else return 0;
		
}

RomanNumber RomanNumber::operator+( const RomanNumber& other )
{
	if(m_isValid && other.m_isValid)	return RomanNumber(m_val+other.m_val);
	else								return RomanNumber();
}

RomanNumber RomanNumber::operator-( const RomanNumber& other )
{
	return *this + RomanNumber(-other.m_val);
}

RomanNumber RomanNumber::operator*( const RomanNumber& other )
{
	if(m_isValid && other.m_isValid)	return RomanNumber(m_val*other.m_val);
	else								return RomanNumber();
}

RomanNumber& RomanNumber::operator=( const RomanNumber& other )
{
	m_isValid = other.m_isValid;
	m_val = other.m_val;

	return *this;
}

bool RomanNumber::operator==( const RomanNumber& other )
{
	return (m_isValid == other.m_isValid) && (m_val == other.m_val);
}

RomanNumber& RomanNumber::operator+=( const RomanNumber& other )
{
	return *this = *this + other;
}

RomanNumber& RomanNumber::operator-=( const RomanNumber& other )
{
	return *this = *this - other;
}

RomanNumber& RomanNumber::operator*=( const RomanNumber& other )
{
	return *this = *this * other;
}

int RomanNumber::value( QChar ch )
{
	if(ch == 'M') return 1000;
	else if(ch == 'D') return 500;
	else if(ch == 'C') return 100;
	else if(ch == 'L') return 50;
	else if(ch == 'X') return 10;
	else if(ch == 'V') return 5;
	else if(ch == 'I') return 1;
	else return 0;
}

int RomanNumber::evaluatePositionString( QString &str )
{
	//int ret = 0;
	//bool subFlag = false;
	//int prevVal = 0;
	//for(QString::const_iterator c = str.end()-1; c >= str.begin(); --c)
	//{
	//	if(subFlag == true) return 0;//только одно вычитание 

	//	int curVal = value(*c);
	//	if(curVal < prevVal)
	//	{
	//		ret -= curVal;
	//		subFlag = true;
	//	}
	//	else ret += curVal;

	//	prevVal = curVal;
	//}

	//return ret;
	if(str.length() > 1)
	{
		QChar first = str[0];
		QString rest = str.mid(1);

		if(first == rest[0])
		{//equal digits like CCC
			if(str.length() > 3) return -1;
			if(rest.length() > 1 && first != rest[1]) return -1;

			return str.length() * value(first);
		}
		else
		{//first + equal digits like DCCC
			if(rest.length() > 3) return -1;
			for(QString::const_iterator c = rest.begin()+1; c != rest.end(); ++c) if(*c != rest[0]) return -1;

			int firstVal = value(first);
			int restVal = value(rest[0]);

			if(firstVal > restVal)	return firstVal + rest.length()*restVal;
			else					return rest.length() == 1 ? restVal-firstVal : -1;//если первая цифра меньше второй, то это вычитание, и справа может быть только одна цифра
		}
		
		//int ret = 0;
		//int prevVal = 0;
		//for(QString::const_iterator c = str.end()-1; c >= str.begin(); --c)
		//{
		//	int curVal = value(*c);
		//	if(curVal < prevVal)
		//	{
		//		ret -= curVal;
		//	}
		//	else ret += curVal;

		//	prevVal = curVal;
		//}

		//return ret;
	}
	else
	{// length <= 1, like X
		return str.length() > 0 ? value(str[0]) : 0;
	}
}
