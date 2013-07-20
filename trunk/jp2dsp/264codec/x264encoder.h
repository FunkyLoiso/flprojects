#ifndef X264ENCODER_H
#define X264ENCODER_H

extern "C"
{
#include "../frogavcodec/include/ffmpeg/avcodec.h"
}
#include "../commonclasses/frame.h"

class X264Encoder
{
public:
	void OpenStream(unsigned int frameW, unsigned int frameH, unsigned int quality);
//	int EncodeFrame(unsigned char *src/*[3]*/, void *dst);//¬озвращает размер закодированных данных
	int EncodeFrame(Frame *src, unsigned char *dst, unsigned int &codedSize);
	void CloseStream();
private:
	AVCodecContext *context;
	AVCodec *encoder;
	AVFrame *frame;

	unsigned int frameW, frameH;
};

#endif /*X264ENCODER_H*/
