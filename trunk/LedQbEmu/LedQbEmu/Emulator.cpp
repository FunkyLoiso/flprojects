#include "Emulator.h"

#include "QBWidget.h"

#define IS_BIT_SET(val, bit) (val & 1 << bit)
#define SET_BIT(val, bit) (val |= 1 << bit)
#define RESET_BIT(val, bit) (val & ~(1 << bit))

Emulator::Emulator(void)
:	m_dsp(NULL), m_digitalWriteDelay_mcs(0), m_latchIsHigh(true)
{
	for(int i = 0; i < 8; ++i) m_lineStates[i].resize(8);
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

	if(m_writeQueue.size() < 9) m_writeQueue.enqueue(val);
}

void Emulator::lightLeds()
{
	if(m_writeQueue.isEmpty()) return;

	//first byte shows which levels are used
	qint8 levels = m_writeQueue.dequeue();

	//butes second to ninth show which leds in each line are used. Byte 2 is for y = 0 and so on
	for(int i = 0; i < 8; ++i)
	{
		if(!m_writeQueue.isEmpty())
		{//we have a byte for this line
			qint8 val = m_writeQueue.dequeue();
	
			for(int c = 0; c < 8; ++c)
			{
				m_lineStates[i].setBit(c, val & (1 << c));
			}
		}
		else m_lineStates[i].fill(false);//we don't have a byte, this line is empty
	}

	//	light leds
	for(int x = 0; x < 8; ++x)
		for(int y = 0; y < 8; ++y)
			for(int z = 0; z < 8; ++z)
			{
				bool isOn = false;
				if(IS_BIT_SET(levels, z))
				{
					isOn = m_lineStates[y].testBit(x);
				}
				m_dsp->setState(x, y, z, isOn);
			}

	//m_dsp->updateGL();
}
