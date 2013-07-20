#ifndef AACDECTHREAD_H
#define AACDECTHREAD_H

#include "../commonclasses/common.h"
#include "../aaccodec/aacdecoder.h"

class AACDecThread : public DecThread
{
public:
	AACDecThread(QSemaphore *outMapFree, QSemaphore *outMapUsed, QMultiMapTS<int, DecodedBuff> *outMap, StreamType stream):DecThread(outMapFree, outMapUsed, outMap, stream){};
	void OpenStream(){decoder.OpenStream();};
	void CloseStream(){decoder.CloseStream();};
	
private:
	void run();
	AACDecoder decoder;
};

#endif /*AACDECTHREAD_H*/
