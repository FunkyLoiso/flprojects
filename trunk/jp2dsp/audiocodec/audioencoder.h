#ifndef AUDIOENCODER_H
#define AUDIOENCODER_H

extern "C"
{
#include "../frogavcodec/include/ffmpeg/avcodec.h"
#include "../frogavcodec/include/ffmpeg/mem.h"
}

class AudioEncoder
{
public:
	void OpenStream(unsigned int nChannels, unsigned  int sampleRate);// акие-то параметры, вроде частоты и качества
	unsigned int EncodeSamples(short *srcSamples, unsigned int nSamples, unsigned char *dst);
	void CloseStream();
private:
	AVCodec *encoder;
	AVCodecContext *context;
};

#endif /*AUDIOENCODER_H*/
