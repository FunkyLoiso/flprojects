#include "ConnectMNKPQPlayerImp.h"
#include "GameState/ConnectMNKPQMove.h"

using namespace AutoBot;

ConnectMNKPQPlayerImp::ConnectMNKPQPlayerImp(void)
{
}

ConnectMNKPQPlayerImp::~ConnectMNKPQPlayerImp(void)
{
}

void ConnectMNKPQPlayerImp::SetRulesManager( const ConnectMNKPQRulesManager* rulesManager) throw(TypeMismatchException)
{
	m_rulesManager = rulesManager;
}
