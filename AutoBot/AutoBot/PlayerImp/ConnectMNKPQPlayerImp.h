#ifndef ConnectMNKPQPlayerImp_h__
#define ConnectMNKPQPlayerImp_h__

#include "PlayerImp.h"
#include "RulesManager\ConnectMNKPQRulesManager.h"

namespace AutoBot
{
	class Move;

	class ConnectMNKPQPlayerImp : public PlayerImp
	{
	public:
		ConnectMNKPQPlayerImp(void);
		virtual ~ConnectMNKPQPlayerImp(void);

	/**	Задать менеджера правил.
	  *	@exception TypeMismatchException, если это менеджер правил от другой игры. */
	virtual void SetRulesManager(const ConnectMNKPQRulesManager*) throw(TypeMismatchException);
	/**	Получить следующий ход на заданном поле. */
	virtual Move GetMove(const Field&) throw(NotInitializedException) = 0;

	protected:
		const ConnectMNKPQRulesManager* m_rulesManager;
	};
}//namespace AutoBot

#endif // ConnectMNKPQPlayerImp_h__
