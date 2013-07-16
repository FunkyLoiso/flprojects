#ifndef READERTHREAD_H
#define READERTHREAD_H

#include <QThread>

class Serial;

class ReaderThread : public QThread
{
	Q_OBJECT

public:
	ReaderThread();
	~ReaderThread();
	bool Start(Serial* port);
	void Stop();

signals:
	void valuesChanged(int v0, int v1, int v2, int v3, int v4, int v5);
	void anglesChanged(double x, double y);

private:
	#define MSG_SIZE 8

	virtual void run();

	Serial* m_port;
	bool m_stopFlag;
	quint8 m_buff[MSG_SIZE];

	int m_curByte;
	int m_pressure[6];
	double m_angle[2];
};

#endif // READERTHREAD_H
