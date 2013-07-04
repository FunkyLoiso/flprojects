#ifndef SerialWrapper_h__
#define SerialWrapper_h__

#include "serial.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class SerialWrapper : public Serial
{
public:
	int available()
	{
		return Input_wait(0)==0 ? 0 : 1;
	}

	int print(const char ch)
	{
		return Write(&ch, 1);
	}

	template<int len> int print(const char str[len])
	{
		return Write(str, len);
	}

	int print(const char* str)
	{
		QByteArray arr = QString(str).toAscii();
		return Write(arr.data(), arr.length());
	}

	int print(int val, int base)
	{
		QByteArray arr = QString::number(val, base).toAscii();
		return Write(arr.data(), arr.length());
	}

	int println()
	{
		return print('\r') + print('\n');
	}

	int println(int val, int base)
	{
		return print(val, base) + println();
	}

	int read()
	{
		int ret;
		int n = Read(&ret, 1);
		if(n < 1) return -1;
		return ret;
	}
};

#endif // SerialWrapper_h__
