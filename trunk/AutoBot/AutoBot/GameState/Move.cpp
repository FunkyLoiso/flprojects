#include "Move.h"
#include "Player/Player.h"

using namespace AutoBot;

Move::Move( const Player* player )
: m_player(player), m_creationTime(QTime::currentTime())
{
}

Move::Move( const Move& other )
{
	*this = other;
}

Move::~Move( void )
{
}

void Move::operator=( const Move& other )
{
	m_player = other.GetPlayer();
	m_creationTime = other.GetTime();
	m_componens = other.Coordiantes();
}

bool Move::operator==( const Move& other )
{
	return *m_player == *(other.GetPlayer()) && m_creationTime == other.GetTime() && m_componens == other.Coordiantes();
}

QString Move::PrintComponents() const
{
	QString result("(");
	for each(CoordType component in m_componens)
	{
		result.append(QString::number(component) + ", ");
	}
	result.chop(2);
	return result + ")";
}
