#ifndef AACDECODER_H
#define AACDECODER_H

extern "C"
{
#include "../frogavcodec/include/ffmpeg/avcodec.h"
#include "../frogavcodec/include/ffmpeg/mem.h"
}

class AACDecoder
{
public:
	void OpenStream();
	unsigned int DecodeSamples(unsigned char *src, unsigned int srcSize, short *dstSamples);
	void CloseStream();
private:
	AVCodec *decoder;
	AVCodecContext *context;
};

#endif /*AACDECODER_H*/
