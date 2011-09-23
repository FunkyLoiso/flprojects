#ifndef OutOfFieldException_h__
#define OutOfFieldException_h__

#include "Exception.h"
#include "GameState/Move.h"
#include "GameState/Field.h"

namespace AutoBot
{

/**	Исключение - ход вне поля */
class OutOfFieldException : public Exception
{
public:
	OutOfFieldException(const Move& move, const Field& field)
	{
		SetCaption("OutOfFieldException");

		QString desc("Cannot make move ");
		desc += move.PrintComponents();
		desc += " on field with dimensions ";
		desc += field.PrintDimensions();

		SetDescription(desc);
	}
};

}//namespace AutoBot

#endif // OutOfFieldException_h__
