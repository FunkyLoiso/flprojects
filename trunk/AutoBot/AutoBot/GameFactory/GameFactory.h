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

/**	Конструирует поле, доску, виджет, менеджера правил */
class GameFactory : public QObject
{
	//Q_OBJECT

public:
	/**	Создать фабрику для игры по заданому набору правил */
	GameFactory(QObject *parent = 0) : QObject(parent) {};
	/**	Деструктор */
	~GameFactory();
	/**	Инициализировать фабрику набором правил соответствующей игры.
	  *	Если правила не от той игры - кидается TypeMismatchException	*/
	virtual void Init(RulesSet*) throw(TypeMismatchException) = 0;
	/** Создать менеждера правил на основе набора правил */
	virtual RulesManager* CreateRulesManager() = 0;
	/** Создать игрока на основе заданной реализации.
	  * В случае ошибки возвращается 0, а в errorDetails пишется описание ошибки */
	virtual Player* CreatePlayer(PlayerImp* imp, QString& errorDetails) = 0;
	/**	Создать класс, отражающий состояние и историю игры */
	virtual GameState* CreateGameState() = 0;

protected:
	RulesSet* m_rulesSet;	///< Набор правил, в соответствии с которыми будут создаваться элементы игры
};

}//namespace AutoBot

#endif // GAMEFACTPY_H
