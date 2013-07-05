#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtGui/QMainWindow>
#include "ui_controller.h"

#include "ServoController.h"
#include "ReaderThread.h"

#include "serial.h"
#include <QElapsedTimer>

class Controller : public QMainWindow
{
	Q_OBJECT

public:
	Controller(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Controller();

public slots:
	void onStart();
	void onSliderLY(int);
	void onSensorData(int);

private:
	virtual void timerEvent(QTimerEvent *);

	Ui::ControllerClass ui;

	ServoController m_srvCtrl;
	ReaderThread m_reader;
	Serial m_readerPort;

	int m_pressure;
	QElapsedTimer m_timer;
};

#endif // CONTROLLER_H
