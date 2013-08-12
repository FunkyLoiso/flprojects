#ifndef SimplePhysicsThread_h__
#define SimplePhysicsThread_h__

#include <QThread>
#include <QElapsedTimer>

class PhysicsEngine;
class Glass;

class SimplePhysicsThread : public QThread
{
	Q_OBJECT
public:
	SimplePhysicsThread(void);
	~SimplePhysicsThread(void);

	void setEngine(PhysicsEngine* engine);
	void setGlass(Glass* glass);

signals:
	void updated();

private:
	PhysicsEngine* m_engine;
	Glass* m_glass;
	QElapsedTimer m_timer;
	quint64 m_lastTime;
	bool m_stopFlag;

	virtual void run();

};
#endif // SimplePhysicsThread_h__
