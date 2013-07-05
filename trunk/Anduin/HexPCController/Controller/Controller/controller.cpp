#include "controller.h"
#include "InputController.h"
#include "Hex_Globals.h"

Controller::Controller(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(ui.bStart, SIGNAL(clicked()), this, SLOT(onStart()));
	connect(ui.sLY, SIGNAL(valueChanged(int)), this, SLOT(onSliderLY(int)));

	m_readerPort.Open("COM5");
	m_readerPort.Set_baud(57600);

	connect(&m_reader, SIGNAL(valueChanged(int)), this, SLOT(onSensorData(int)));

	m_reader.Start(&m_readerPort);

	m_srvCtrl.Start();
	g_InControlState.SelectedLeg = 1;

	startTimer(1);
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
	m_timer.restart();
}

void Controller::onSliderLY(int val)
{
	//g_InputController.setLY(val);
	g_InControlState.SLLeg.y = val;
}

void Controller::onSensorData(int val)
{
	ui.sSensorValue->setValue(val);
	m_pressure = val;
}

void Controller::timerEvent(QTimerEvent *)
{
	static int optimalPresure = 350;
	static double pos = 0;

	double secsElapsed = (double)m_timer.nsecsElapsed()/1000000000;
	static const double maxSpeed = 60;
	double speed = 0;
	//if(m_pressure > 500) speed = -maxSpeed;
	//else if(m_pressure < 300) speed = maxSpeed;
	speed = optimalPresure - m_pressure;

	pos += speed*secsElapsed;

	if(pos > 40) pos = 40;
	if(pos < -40) pos = -40;

	g_InControlState.SLLeg.y = pos;

	m_timer.restart();
}

