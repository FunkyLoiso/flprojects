#include "ConnectMNKPQScriptPlayerImp.h"

#include "GameState/ConnectMNKPQMove.h"
#include "GameState/Field.h"

using namespace AutoBot;

ConnectMNKPQScriptPlayerImp::ConnectMNKPQScriptPlayerImp(void)
{
}

ConnectMNKPQScriptPlayerImp::~ConnectMNKPQScriptPlayerImp(void)
{
}

void ConnectMNKPQScriptPlayerImp::init() throw(ScriptErrorException)
{
	throw std::exception("The method or operation is not implemented.");
	/* ��� �� �������� ������� � ���� */
}

Move ConnectMNKPQScriptPlayerImp::GetMove( const Field& ) throw(NotInitializedException)
{
	throw std::exception("The method or operation is not implemented.");
	/* ��� �� ��������� ����.. */
	return ConnectMNKPQMove();
}
