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
	m_port->Input_discard();
	quint8 startMsg = 97;
	m_port->Write(&startMsg, 1);
	QThread::start();
	return true;
}

void ReaderThread::Stop()
{
	m_stopFlag = true;
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
	static quint8 buff[2];
	static quint8* buffPtr = buff;
	while(!m_stopFlag)
	{
		int r;

		r = m_port->Input_wait(1);
		if (r > 0)
		{
			int readSize = buffPtr - buff;
			r = m_port->Read(buffPtr, 2 - readSize);
			if (r + readSize < 2)
			{
				buffPtr += r;
				continue;
			}
			else
			{
				int v2 = buff[0] | (short(buff[1] & 0x3) << 8);

				emit valueChanged(func2(v2));
				buffPtr = buff;
			}
		}
	}
}
