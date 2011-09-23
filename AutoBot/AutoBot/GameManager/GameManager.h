#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>

class GameManager : public QObject
{
	Q_OBJECT

public:
	GameManager(QObject *parent);
	~GameManager();

private:
	
};

#endif // GAMEMANAGER_H
