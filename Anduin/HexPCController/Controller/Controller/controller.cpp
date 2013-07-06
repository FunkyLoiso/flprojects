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

	connect(&m_reader, SIGNAL(valueChanged(int)), this, SLOT(onSensorData(int)));

	m_reader.Start(&m_readerPort);

	//g_InControlState.fHexOn = true;
	//g_InputController.setLY(100);
	m_srvCtrl.Start();
	//g_InControlState.SelectedLeg = 1;
	//g_InControlState.BalanceMode = true;

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
	g_InputController.setLY(val);
	//g_InControlState.SLLeg.y = val;
}

void Controller::onSensorData(int val)
{
	ui.sSensorValue->setValue(val);
	m_pressure = val;
}

//void Controller::timerEvent(QTimerEvent *)
//{
	//static const int optimalPressure = 400;
	//static const int intervalHalf = 100;
	//static VPid pid(0.1, 0.0, 0.0, 0.01, -100.0, 100.0);
	//static double pos = 0;

	//int delta = optimalPressure-m_pressure;
	//if(qAbs(delta) > intervalHalf) delta = (qAbs(delta)-intervalHalf)*(delta < 0 ? -1 : 1);
	//else delta = 0;

	//double speed = 0;
	//speed = pid.getValue(delta);
	//ui.sSpeed->setValue(speed);

	//pos += speed*0.01;

	//if(pos > 40) pos = 40;
	//if(pos < -40) pos = -40;

	//g_InControlState.SLLeg.y = pos;
	//ui.sLY->setValue(pos);

	//m_timer.restart();
//}

//void Controller::timerEvent(QTimerEvent *)
//{
//	static double speed = 200.0;
//	static double startPos = 0.0;
//
//	static bool stop = false;
//
//	double secsElapsed = (double)m_timer.nsecsElapsed()/1000000000;
//	double pos = startPos + secsElapsed * speed;
//
//	if(m_pressure < 200) stop = false;
//	if(m_pressure > 200)
//	{
//		m_timer.restart();
//		if(stop == false)
//		{
//			startPos = pos = g_InControlState.SLLeg.y - speed*0.15;
//			stop = true;
//		}
//	}	
//
//	if(pos > 40.0)
//	{
//		pos = startPos = 40.0;
//		speed *= -1;
//		m_timer.restart();
//	}
//	else if(pos < -40)
//	{
//		pos = startPos = -40.0;
//		speed *= -1;
//		m_timer.restart();
//	}
//
//	g_InControlState.SLLeg.y = pos;
//
//	//m_timer.restart();
//}

extern long			FloorLevel[6];

void Controller::timerEvent(QTimerEvent *)
{
	static bool contact = false;
	if(m_pressure > 300 && !contact)
	{
		contact = true;
		SSCSerial.println("STOP 6");
		SSCSerial.println("STOP 5");
		SSCSerial.println("QP 5");
		SSCSerial.Input_wait(5000);

		quint8 buff;
		int read = SSCSerial.Read(&buff, 1);
		//Q_ASSERT(read == 1);

		int p5 = buff*10;
		double angleDeg = double(p5-592)*991/10000;
		double delta = 90.0 - angleDeg;

		int y = cXXFemurLength * qSin(delta*3.1415/180.0);

		FloorLevel[cLM] = y;
	}
	if(m_pressure < 300 && contact) contact = false;
}
