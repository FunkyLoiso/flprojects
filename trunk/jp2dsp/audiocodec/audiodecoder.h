#ifndef AUDIODECODER_H
#define AUDIODECODER_H

extern "C"
{
#include "../frogavcodec/include/ffmpeg/avcodec.h"
#include "../frogavcodec/include/ffmpeg/mem.h"
}

class AudioDecoder
{
public:
	void OpenStream();
	unsigned int DecodeSamples(unsigned char *src, unsigned int srcSize, short *dstSamples);
	void CloseStream();
private:
	AVCodec *decoder;
	AVCodecContext *context;
};

#endif /*AUDIODECODER_H*/
