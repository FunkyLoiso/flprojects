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
	void valueChanged(int v2);

private:
	virtual void run();

	Serial* m_port;
	bool m_stopFlag;
};

#endif // READERTHREAD_H
