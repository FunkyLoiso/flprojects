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

	/**	������ ��������� ������.
	  *	@exception TypeMismatchException, ���� ��� �������� ������ �� ������ ����. */
	virtual void SetRulesManager(const ConnectMNKPQRulesManager*) throw(TypeMismatchException);
	/**	�������� ��������� ��� �� �������� ����. */
	virtual Move GetMove(const Field&) throw(NotInitializedException) = 0;

	protected:
		const ConnectMNKPQRulesManager* m_rulesManager;
	};
}//namespace AutoBot

#endif // ConnectMNKPQPlayerImp_h__
