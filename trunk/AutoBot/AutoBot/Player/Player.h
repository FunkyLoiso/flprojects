#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "PlayerImp\PlayerImp.h"
#include "RulesManager\RulesManager.h"

namespace AutoBot
{
class Move;
class Field;

class TypeMismatchException;
class NotInitializedException;

/**	����� */
class Player
{
public:
	Player(QString name) : m_imp(NULL), m_inited(false) {};
	virtual ~Player() {};

	QString GetName() const { return m_name; }

	virtual bool operator==(const Player&) const = 0;

	/**	���������������� ������ �����������, �������� ��� �������� ������
	  *	@exception TypeMismatchException, ���� ���������� ��� �������� ��� ������ ����*/
	void Init(PlayerImp* imp, const RulesManager* rulesManager) throw(TypeMismatchException);
	/**	�������� ����� ��� ������ �� �������� �����. */
	Move GetMove(const Field& field) const throw(NotInitializedException);

protected:
	/**	���������� ����������.
	  *	���������� � Init
	  *	@exception TypeMismatchException ���� ���������� �� ���� ���� */
	virtual void checkImplementation(const PlayerImp*) const throw(TypeMismatchException) = 0;

private:
	bool m_inited;
	PlayerImp* m_imp;

	QString m_name;
};

}//namespace AutoBot

#endif // PLAYER_H
