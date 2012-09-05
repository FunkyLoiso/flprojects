#ifndef Emulator_h__
#define Emulator_h__

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QBitArray>

class QBWidget;

class Emulator : private QThread
{
public:
	Emulator(void);
	~Emulator(void);

	void setDisplay(QBWidget* display) {m_dsp = display;};

	qint64 digitalWriteDelay_mcs() const { return m_digitalWriteDelay_mcs; }
	void setDigitalWriteDelay_mcs(qint64 val) { m_digitalWriteDelay_mcs = val; }

	void setLatch(bool toHigh);
	void writeByte(qint8 val);

private:
	//void run();
	void lightLeds();
	QBWidget* m_dsp;

	QMutex m_operationMutex;

	bool m_latchIsHigh;
	qint64 m_digitalWriteDelay_mcs;

	QQueue<qint8> m_writeQueue;

	QBitArray m_levelStats;
	QBitArray m_lineStates[8];

	//bool m_endFlag;
};
#endif // Emulator_h__
