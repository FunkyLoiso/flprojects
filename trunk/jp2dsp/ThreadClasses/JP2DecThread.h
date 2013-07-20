#ifndef JP2DECSTREAM_H
#define JP2DECSTREAM_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "../commonclasses/common.h"

//#define USE_LuraWave
#define USE_J2K_Codec

#ifdef USE_J2K_Codec
#include "../J2K-Codec/j2k-codec.h"
#endif
#ifdef USE_LuraWave
#include "../LuraWave/lwf_jp2.h"



#endif

class JP2DecThread : public DecThread
{
public:
	JP2DecThread(QSemaphore *outMapFree, QSemaphore *outMapUsed, QMultiMapTS<int, DecodedBuff> *outMap, StreamType stream):DecThread(outMapFree, outMapUsed, outMap, stream)
	{
		oddWorker = new JP2Worker(&oddSem, &doneSem, true);
		evenWorker = new JP2Worker(&evenSem, &doneSem, false);
		oddWorker->start();
		evenWorker->start();

		oddWorker->setPriority(QThread::LowPriority);
		evenWorker->setPriority(QThread::LowPriority);
	};
	void OpenStream(){};
	void CloseStream(){};
private:
	void run();

	unsigned int reduceVal;
	QWaitCondition waitCondition;
	QSemaphore oddSem, evenSem, doneSem;

	class JP2Worker : public QThread
	{
	public:
		JP2Worker(QSemaphore *waitSem, QSemaphore *doneSem, bool isOdd): terminateFlag(false), waitSem(waitSem), doneSem(doneSem), isOdd(isOdd){};
		void SetData(CodedBuff *data, unsigned char *outBuff){this->data = data, this->outBuff = outBuff;};

		bool terminateFlag;
		bool ready;
	private:
		QSemaphore *waitSem, *doneSem;
		unsigned char *outBuff;
		bool isOdd;

		CodedBuff *data;

		void run();

	} *oddWorker, *evenWorker;
};

#endif /*JP2DECSTREAM_H*/
