#ifndef DISPATCHERTHREAD_H
#define DISPATCHERTHREAD_H

#include <QThread>
#include <QSemaphore>
#include <QMap>
#include <QMutex>

#include "../commonclasses/common.h"
#include "../commonclasses/clock.h"
//#include "../frogavcodec/include/ffmpeg/avcodec.h"
//#include "../commonclasses/player.h"

template <typename keyT, typename valT>
class QMultiMapTS;

class DispatcherThread : public QThread
{
	Q_OBJECT
public:
	DispatcherThread(QMap<StreamType, QSemaphore *> *outMapFree, QSemaphore *outMapUsed, QMultiMapTS<int, DecodedBuff> *outMap, QMap<StreamType, DecoderPair> *renderers):terminateFlag(false), outMapFree(outMapFree), outMapUsed(outMapUsed), outMap(outMap), renderers(renderers), paused(false), waitingForData(false){};
	void Pause(bool pause);
	void SetClock(int time_ms){clock.Set(time_ms);};

	bool terminateFlag;
	bool waitingForData;

signals:
	void SetProgressVal(int);

private:
	void run();
	QMap<StreamType, QSemaphore *> *outMapFree;
	QSemaphore *outMapUsed;

	QMultiMapTS<int, DecodedBuff> *outMap;
	QMap<StreamType, DecoderPair> *renderers;
	Clock clock;
	QMutex pauseMutex;

	bool paused;
	float renderingSpeed;
	int minFramesSinceDrop;
	int framesDropped;
};

#endif /*DISPATCHERTHREAD_H*/
