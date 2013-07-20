#ifndef AUDIODECTHREAD_H
#define AUDIODECTHREAD_H

#include "../commonclasses/common.h"
#include "../audiocodec/audiodecoder.h"

class AudioDecThread : public DecThread
{
public:
	AudioDecThread(QSemaphore *outMapFree, QSemaphore *outMapUsed, QMultiMapTS<int, DecodedBuff> *outMap, StreamType stream):DecThread(outMapFree, outMapUsed, outMap, stream){};
	void OpenStream(){decoder.OpenStream();};
	void CloseStream(){decoder.CloseStream();};
	
private:
	void run();
	AudioDecoder decoder;
};

#endif /*AUDIODECTHREAD_H*/
