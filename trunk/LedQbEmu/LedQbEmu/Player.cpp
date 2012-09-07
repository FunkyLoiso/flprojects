#include "Player.h"

#include "Emulator.h"

Player::Player(Emulator* emu, QObject* parent)
: m_emu(emu), QThread(parent)
{
	start();
}

Player::~Player(void)
{
}

void Player::run()
{
	for(;;)
	{
		//msleep(50);

		static int x = 0, y = 0, z = 0;
	
		if(++x > 7) x = 0;
		if(++y > 7) y = 0;
		if(++z > 7) z = 0;
	
		m_emu->setLatch(false);
		m_emu->writeByte(1 << z);
		for(int i = 0; i < y; ++i)
		{
			m_emu->writeByte(0);
		}
		m_emu->writeByte((1 << x+1)-1);
		m_emu->setLatch(true);
	}
}
