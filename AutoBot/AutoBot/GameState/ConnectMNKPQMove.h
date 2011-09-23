#ifndef ConnectMNKPQMove_h__
#define ConnectMNKPQMove_h__

#include "Move.h"

namespace AutoBot
{
	class Player;

	class ConnectMNKPQMove : public Move
	{
	public:
		ConnectMNKPQMove(CoordType x, CoordType y, const Player* player) : Move(player)
		{
			init(x, y);
		}

		/**	 онструктор недействительного хода */
		ConnectMNKPQMove() : Move(NULL)
		{
			init(0, 0);
		}

		virtual ~ConnectMNKPQMove(void)
		{
		}

		CoordType x() { return m_componens[0]; };
		CoordType y() { return m_componens[1]; };

	private:
		void init(CoordType x, CoordType y) 
		{
			m_componens.resize(2);
			m_componens[0] = x;
			m_componens[1] = y;
		}
	};
}//namespace AutoBot

#endif // ConnectMNKPQMove_h__
