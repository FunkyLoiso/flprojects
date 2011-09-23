#ifndef NotInitializedException_h__
#define NotInitializedException_h__

#include "Exception.h"

namespace AutoBot
{

class NotInitializedException : public Exception
{
public:

	NotInitializedException(QString methodToInit)
	{
		SetCaption("NotInitializedException");
		SetDescription("Object is not properly initialized. Please, call method \"" + methodToInit + "\"");
	}

	virtual ~NotInitializedException(void)
	{
	}
};

}//namespace AutoBot

#endif // NotInitializedException_h__
