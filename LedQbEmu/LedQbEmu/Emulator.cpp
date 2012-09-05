#include "Emulator.h"

#include "QBWidget.h"

Emulator::Emulator(void)
:	m_dsp(NULL), m_digitalWriteDelay_mcs(0), m_latchIsHigh(true)
{
	m_levelStats.resize(8);
	for(int i = 0; i < 8; ++i) m_lineStates[i].resize(8);
	//start();
}

Emulator::~Emulator(void)
{

}

void Emulator::setLatch(bool toHigh)
{
	QMutexLocker locker(&m_operationMutex);
	usleep(m_digitalWriteDelay_mcs);

	if(m_latchIsHigh && !toHigh)
	{// high to low
		m_latchIsHigh = false;
	}
	else if(!m_latchIsHigh && toHigh)
	{// low to high
		lightLeds();
	}
}

void Emulator::writeByte(qint8 val)
{
	QMutexLocker locker(&m_operationMutex);
	usleep(14 * m_digitalWriteDelay_mcs);

	m_writeQueue.enqueue(val);
}

void Emulator::lightLeds()
{
	if(m_writeQueue.isEmpty()) return;

	// move values from writeQueue to bit arrays
	qint8 levels = m_writeQueue.dequeue();
	for(int i = 0; i < 8; ++i)
	{
		//if(levels & (1 << i)) m_levelStats.setBit(i);
		m_levelStats.setBit(i, levels & (1 << i));
	}
	for(int i = 0; !m_writeQueue.isEmpty(); ++i)
	{
		qint8 val = m_writeQueue.dequeue();

		for(int c = 0; c < 8; ++c)
		{
			m_lineStates[i].setBit(c, val & (1 << c));
		}
	}

	//	light leds
	for(int x = 0; x < 8; ++x)
		for(int y = 0; y < 8; ++y)
			for(int z = 0; z < 8; ++z)
			{
				bool isOn = false;
				if(m_levelStats.testBit(z))
				{
					isOn = m_lineStates[y].testBit(x);
				}
				m_dsp->setState(x, y, z, isOn);
			}

	m_dsp->updateGL();
}
