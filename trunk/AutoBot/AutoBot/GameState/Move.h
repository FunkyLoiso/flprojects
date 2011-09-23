#ifndef Move_h__
#define Move_h__

#include <QTime>
#include <QVector>

namespace AutoBot
{
class Player;

/**	Ход одного игрока в игре */
class Move
{
public:
	typedef unsigned int CoordType;
	typedef QVector<CoordType> MoveCoords;

	/**	@param player Игрок, сделавший ход. Равенство нулю - признак недействительного хода */
	Move(const Player* player = NULL);
	Move(const Move& other);
	
	virtual ~Move(void);

	void operator=(const Move& other);
	bool operator==(const Move& other);

	/**	Указатель на игрока, сделавшего ход */
	const Player* GetPlayer() const { return m_player; }
	/**	Время создания хода */
	QTime GetTime() const {return m_creationTime; }
	/**	Список компонентов хода */
	MoveCoords Coordiantes() const {return m_componens; }
	/**	Распечатать координаты хода */
	QString PrintComponents() const;
	/**	Действителен ли ход? */
	bool IsValid() const {return m_player != NULL;};

protected:
	MoveCoords m_componens;

private:
	const Player* m_player;
	QTime m_creationTime;
};

}//namespace AutoBot

#endif // Move_h__
