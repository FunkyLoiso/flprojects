#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtGui/QMainWindow>
#include "ui_controller.h"

#include "ServoController.h"

class Controller : public QMainWindow
{
	Q_OBJECT

public:
	Controller(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Controller();

public slots:
	void onStart();
	void onSliderLY(int);

private:
	Ui::ControllerClass ui;

	ServoController m_srvCtrl;
};

#endif // CONTROLLER_H
