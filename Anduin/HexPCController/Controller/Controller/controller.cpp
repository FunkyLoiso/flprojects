#include "controller.h"
#include "InputController.h"
#include "Hex_Globals.h"

#include <QtCore/qmath.h>

Controller::Controller(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(ui.bStart, SIGNAL(clicked()), this, SLOT(onStart()));
	connect(ui.sLY, SIGNAL(valueChanged(int)), this, SLOT(onSliderLY(int)));

	m_readerPort.Open("COM5");
	m_readerPort.Set_baud(57600);

	connect(&m_reader, SIGNAL(valuesChanged(int, int, int, int, int, int)), this, SLOT(onSensorData(int, int, int, int, int, int)));
	connect(&m_reader, SIGNAL(anglesChanged(double, double)), this, SLOT(onAnglesData(double, double)));

	m_reader.Start(&m_readerPort);

	g_InControlState.fHexOn = true;
	//g_InputController.setLY(100);
	m_srvCtrl.Start();
	//g_InControlState.SelectedLeg = 1;
	//g_InControlState.BalanceMode = true;

	//startTimer(1);
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
	//g_InControlState.SLLeg.y = val;
}

void Controller::onSensorData(int v0, int v1, int v2, int v3, int v4, int v5)
{
	ui.sSensorLF->setValue( g_InControlState.sensorValue[cLF] = v5 );
	ui.sSensorLM->setValue( g_InControlState.sensorValue[cLM] = v3 );
	ui.sSensorLR->setValue( g_InControlState.sensorValue[cLR] = v0 );
	ui.sSensorRF->setValue( g_InControlState.sensorValue[cRF] = v4 );
	ui.sSensorRM->setValue( g_InControlState.sensorValue[cRM] = v2 );
	ui.sSensorRR->setValue( g_InControlState.sensorValue[cRR] = v1 );
}

double filter(double val, double prevFiltered, double alpha)
{
	return prevFiltered  + alpha * (val - prevFiltered);
}

void Controller::onAnglesData(double x, double y)
{
	static double alpha = 0.002;
	x = filter(x, g_InControlState.accelAngles[0], alpha);
	y = filter(y, g_InControlState.accelAngles[1], alpha);

	g_InControlState.accelAngles[0] = x;
	g_InControlState.accelAngles[1] = y;

	ui.sAngleX->setValue(x*10);
	ui.sbAngleX->setValue(x);

	ui.sAngleY->setValue(y*10);
	ui.sbAngleY->setValue(y);
}
