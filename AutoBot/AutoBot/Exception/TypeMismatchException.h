#ifndef TypeMismatchException_h__
#define TypeMismatchException_h__

#include "Exception.h"

namespace AutoBot
{

/**	Несовпадение типов */
class TypeMismatchException : public Exception
{
public:
	TypeMismatchException(const QString& expected, const QString& actual)
	{
		init(expected, actual);
	}

	TypeMismatchException(const type_info& expected, const type_info& actual)
	{
		init(expected.name(), actual.name());
	}

private:
	void init(const QString& expected, const QString& actual)
	{
		SetCaption("TypeMismatchException");
		SetDescription(QString("Expected type \"{0}\" but got \"{1}\"").arg(expected).arg(actual));
	}
};

}//namespace AutoBot

#endif // TypeMismatchException_h__