#ifndef ConnectMNKPQScriptPlayerImp_h__
#define ConnectMNKPQScriptPlayerImp_h__

#include "ConnectMNKPQPlayerImp.h"
#include "IntellectSource/ScriptSource.h"

namespace AutoBot
{
class ScriptErrorException;

class ConnectMNKPQScriptPlayerImp : public ConnectMNKPQPlayerImp, private ScriptSource
{
public:
	ConnectMNKPQScriptPlayerImp(void);
	virtual ~ConnectMNKPQScriptPlayerImp(void);

	/**	�������� ��� �� �������� ���� */
	virtual Move GetMove(const Field&) throw(NotInitializedException);

protected:
	/**	��������� ������, ������������������ */
	virtual void init() throw(ScriptErrorException);
};
}//namespace AutoBot

#endif // ConnectMNKPQScriptPlayerImp_h__
