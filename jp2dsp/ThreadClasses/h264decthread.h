#ifndef H264DECTHREAD_H
#define H264DECTHREAD_H

#include "../264codec/h264decoder.h"
#include "../commonclasses/common.h"

class H264DecThread : public DecThread
{
	//Q_OBJECT
public:
	H264DecThread(QSemaphore *outMapFree, QSemaphore *outMapUsed, QMultiMapTS<int, DecodedBuff> *outMap, StreamType stream):DecThread(outMapFree, outMapUsed, outMap, stream){};
	void OpenStream(){decoder.OpenStream();};
	void CloseStream(){decoder.CloseStream();};
	
private:
	void run();
	H264Decoder decoder;
};

#endif /*H264DECTHREAD_H*/
