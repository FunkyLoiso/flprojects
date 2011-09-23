#ifndef Move_h__
#define Move_h__

#include <QTime>
#include <QVector>

namespace AutoBot
{
class Player;

/**	��� ������ ������ � ���� */
class Move
{
public:
	typedef unsigned int CoordType;
	typedef QVector<CoordType> MoveCoords;

	/**	@param player �����, ��������� ���. ��������� ���� - ������� ����������������� ���� */
	Move(const Player* player = NULL);
	Move(const Move& other);
	
	virtual ~Move(void);

	void operator=(const Move& other);
	bool operator==(const Move& other);

	/**	��������� �� ������, ���������� ��� */
	const Player* GetPlayer() const { return m_player; }
	/**	����� �������� ���� */
	QTime GetTime() const {return m_creationTime; }
	/**	������ ����������� ���� */
	MoveCoords Coordiantes() const {return m_componens; }
	/**	����������� ���������� ���� */
	QString PrintComponents() const;
	/**	������������ �� ���? */
	bool IsValid() const {return m_player != NULL;};

protected:
	MoveCoords m_componens;

private:
	const Player* m_player;
	QTime m_creationTime;
};

}//namespace AutoBot

#endif // Move_h__
