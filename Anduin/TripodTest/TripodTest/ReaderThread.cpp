#include "ReaderThread.h"
#include "serial.h"

#define BAUD 115200

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

short func(short x)
{
	return -87859.04084 / (double(x) - 836.8694687);
}

void ReaderThread::run()
{
	//static quint8 buf[1024];
	static quint8 buff[4];
	static quint8* buffPtr = buff;
	static int channel = 0;
	while(!m_stopFlag)
	{
		int r;

		//printf("Idle event\n");
		r = m_port->Input_wait(40);
		if (r > 0)
		{
			int readSize = buffPtr - buff;
			r = m_port->Read(buffPtr, 4 - readSize);
			if (r + readSize < 4)
			{
				buffPtr += r;
				continue;
			}
			else
			{
				short v0 = buff[0] | (short(buff[1] & 0x3) << 8);
				short v3 = (buff[1] >> 2) | (short(buff[2] & 0xf) << 6);
				short v5 = (buff[2] >> 4) | (short(buff[3] & 0x3f) << 4);

				emit valuesCahnged(func(v0), func(v3), func(v5));
				buffPtr = buff;
			}
		}
	}
}
