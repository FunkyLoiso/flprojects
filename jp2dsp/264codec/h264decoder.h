#ifndef H264DECODER_H
#define H264DECODER_H

#include "../frogavcodec/include/ffmpeg/avcodec.h"
#include "../frogavcodec/include/ffmpeg/mem.h"
#include "../frogavcodec/include/ffmpeg/swscale.h"

class Frame;

class H264Decoder
{
public:
	H264Decoder(): decoder(0), context(0), frame(0){};
	void OpenStream();
	Frame *DecodeFrame(unsigned char *src, unsigned int srcSize);
	void GetImageDims(unsigned int &width, unsigned int &height);
	void CloseStream();
private:
	AVCodec *decoder;
	AVCodecContext *context;
	AVFrame *frame;
};

#endif /*H264DECODER_H*/
