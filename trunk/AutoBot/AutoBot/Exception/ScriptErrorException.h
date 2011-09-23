#ifndef ScriptErrorException_h__
#define ScriptErrorException_h__

#include "Exception.h"

namespace AutoBot
{
	class ScriptErrorException : public Exception
	{
	public:
	
		ScriptErrorException(const QString& exception, int lineNumber)
		{
			SetCaption("ScriptErrorException");
			SetDescription(exception + "\n\nLine number: " + QString::number(lineNumber));
		}
	
		virtual ~ScriptErrorException(void)
		{
		}
	};
}//namespace AutoBot

#endif // ScriptErrorException_h__
