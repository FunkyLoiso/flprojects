#ifndef PlayerImp_h__
#define PlayerImp_h__

namespace AutoBot
{
class RulesManager;
class Field;
class Move;
class TypeMismatchException;
class NotInitializedException;

/**	Реализация игрока */
class PlayerImp
{
public:
	PlayerImp(void)
	{
	}

	virtual ~PlayerImp(void)
	{
	}

	/**	Задать менеджера правил.
	  *	@exception TypeMismatchException, если это менеджер правил от другой игры. */
	virtual void SetRulesManager(const RulesManager*) throw(TypeMismatchException) = 0;
	/**	Получить следующий ход на заданном поле. */
	virtual Move GetMove(const Field&) throw(NotInitializedException) = 0;
};

}//namespace AutoBot

#endif // PlayerImp_h__
