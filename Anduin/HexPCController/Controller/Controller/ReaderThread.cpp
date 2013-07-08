#include "ReaderThread.h"
#include "serial.h"
#include <qmath.h>

#define BAUD 57600

ReaderThread::ReaderThread() : m_stopFlag(false)
{

}

ReaderThread::~ReaderThread()
{
	Stop();
}

bool ReaderThread::Start(Serial* port)
{
	if(!port->Is_open()) return false;
	m_port = port;
	m_port->Set_baud(BAUD);
	Stop();
	m_stopFlag = false;
	m_port->Input_discard();
	Sleep(100);

	m_buffPtr = m_buff;

	quint8 startMsg = 97;
	QThread::start();
	m_port->Write(&startMsg, 1);

	return true;
}

void ReaderThread::Stop()
{
	m_stopFlag = true;
	quint8 stopMsg = 99;
	m_port->Write(&stopMsg, 1);
	QThread::wait();
}

//48.395394 * x^0.401336
int func0(int x)
{
	//return qExp((x + 520.1946406)/177.7705576);
	//return qPow((double(x)/48.395394), 1.0/0.401336);
	return x;
}
int func2(int x)
{
	//if(x < 635) return 0;
	//if(x < 750)	return -28691/(x-812);
	//else		return double(x-714)/0.0868929;
	return x;
}
int func3(int x)
{
	//return qExp((x + 92.2632788)/129.8362373);
	return x;
}
int func5(int x)
{
	//return qExp((x + 994.6591453)/258.7622672);
	return x;
}


void ReaderThread::run()
{

	while(!m_stopFlag)
	{
		int r;

		r = m_port->Input_wait(1);
		if (r > 0)
		{
			int readSize = m_buffPtr - m_buff;
			r = m_port->Read(m_buffPtr, MSG_SIZE - readSize);
			if (r + readSize < MSG_SIZE)
			{
				m_buffPtr += r;
				continue;
			}
			else
			{
				emit valuesChanged(m_buff[0]*4, m_buff[1]*4, m_buff[2]*4, m_buff[3]*4, m_buff[4]*4, m_buff[5]*4);
				m_buffPtr = m_buff;
			}
		}
	}
}
