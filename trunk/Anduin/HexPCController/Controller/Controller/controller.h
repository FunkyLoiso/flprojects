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
	void onSensorData(int, int, int, int, int, int);
	void onAnglesData(double, double);

private:
	//virtual void timerEvent(QTimerEvent *);

	Ui::ControllerClass ui;

	ServoController m_srvCtrl;
	ReaderThread m_reader;
	Serial m_readerPort;
};

#endif // CONTROLLER_H
