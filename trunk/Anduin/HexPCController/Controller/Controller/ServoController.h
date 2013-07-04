#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <QThread>
#include <QElapsedTimer>
#include "InputController.h"

class ServoController : public QThread
{
	Q_OBJECT

public:
	ServoController(QObject *parent = NULL);
	~ServoController();

	void Start();
	void Stop();

	static InputController& ctrl();

private:
	bool m_stopFlag;

	virtual void run();

	QElapsedTimer m_timer;

	void setup();
	void loop();

	//virtual void timerEvent(QTimerEvent *);


	
};

#endif // SERVOCONTROLLER_H
