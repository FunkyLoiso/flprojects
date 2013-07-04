#include "controller.h"
#include "InputController.h"
#include "Hex_Globals.h"

Controller::Controller(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(ui.bStart, SIGNAL(clicked()), this, SLOT(onStart()));
	connect(ui.sLY, SIGNAL(valueChanged(int)), this, SLOT(onSliderLY(int)));

	m_srvCtrl.Start();
}

Controller::~Controller()
{
	m_srvCtrl.Stop();
}

void Controller::onStart()
{
	bool& on = g_InControlState.fHexOn;
	if(on)	ui.bStart->setText("Start");
	else	ui.bStart->setText("Stop");

	on = !on;
}

void Controller::onSliderLY(int val)
{
	g_InputController.setLY(val);
}

