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

	void StopTripodOnContact(uint8_t f1, uint8_t, uint8_t f3);
	void TripodHorizontalBalance(uint8_t middleLeg);

	//virtual void timerEvent(QTimerEvent *);


	
};

#endif // SERVOCONTROLLER_H
