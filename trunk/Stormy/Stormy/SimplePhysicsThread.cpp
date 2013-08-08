#include "SimplePhysicsThread.h"

#include "PhysicsEngine.h"
#include <windows.h>

SimplePhysicsThread::SimplePhysicsThread(void)
: m_engine(NULL), m_glass(NULL), m_stopFlag(false)
{
}

SimplePhysicsThread::~SimplePhysicsThread(void)
{
	m_stopFlag = true;
	this->wait();
}

void SimplePhysicsThread::run()
{
	Q_CHECK_PTR(m_engine);
	Q_CHECK_PTR(m_glass);

	m_timer.start();
	m_lastTime = 0;
	while(!m_stopFlag)
	{
		quint64 curTime = m_timer.nsecsElapsed();
		m_engine->update(m_glass, (curTime-m_lastTime)/1000000000.0f);
		m_lastTime = curTime;
		emit updated();

		//Sleep(1);
	}
}

void SimplePhysicsThread::setEngine(PhysicsEngine* engine)
{
	m_engine = engine;
}

void SimplePhysicsThread::setGlass(Glass* glass)
{
	m_glass = glass;
}
