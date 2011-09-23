#ifndef GAMEFACTPY_H
#define GAMEFACTPY_H

#include <QObject>
#include <QString>

#include "RulesSet\RulesSet.h"
#include "RulesManager\RulesManager.h"
#include "Player\Player.h"
#include "PlayerImp\PlayerImp.h"
#include "Exception\Exception.h"
#include "GameState\GameState.h"

namespace AutoBot
{

/**	������������ ����, �����, ������, ��������� ������ */
class GameFactory : public QObject
{
	//Q_OBJECT

public:
	/**	������� ������� ��� ���� �� �������� ������ ������ */
	GameFactory(QObject *parent = 0) : QObject(parent) {};
	/**	���������� */
	~GameFactory();
	/**	���������������� ������� ������� ������ ��������������� ����.
	  *	���� ������� �� �� ��� ���� - �������� TypeMismatchException	*/
	virtual void Init(RulesSet*) throw(TypeMismatchException) = 0;
	/** ������� ��������� ������ �� ������ ������ ������ */
	virtual RulesManager* CreateRulesManager() = 0;
	/** ������� ������ �� ������ �������� ����������.
	  * � ������ ������ ������������ 0, � � errorDetails ������� �������� ������ */
	virtual Player* CreatePlayer(PlayerImp* imp, QString& errorDetails) = 0;
	/**	������� �����, ���������� ��������� � ������� ���� */
	virtual GameState* CreateGameState() = 0;

protected:
	RulesSet* m_rulesSet;	///< ����� ������, � ������������ � �������� ����� ����������� �������� ����
};

}//namespace AutoBot

#endif // GAMEFACTPY_H
