#ifndef Player_h__
#define Player_h__

#include <QThread>

class Emulator;

class Player : public QThread
{
public:
	Player(Emulator* emu, QObject* parent = NULL);
	~Player(void);

private:
	void run();

	Emulator* m_emu;
};
#endif // Player_h__
