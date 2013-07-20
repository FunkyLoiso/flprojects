#ifndef FRAME_H
#define FRAME_H

#include "../frogavcodec/include/ffmpeg/avutil.h"
#include "../frogavcodec/include/ffmpeg/avcodec.h"

class Frame
{
public:
	Frame(AVFrame *avFrame, PixelFormat pixFmt, unsigned int width, unsigned int height) : pixFmt(pixFmt), width(width), height(height)
	{
		internalAVFrame = avcodec_alloc_frame();
		buffer = (unsigned char *)malloc(avpicture_get_size(pixFmt, width, height));
		avpicture_fill((AVPicture *)internalAVFrame, (unsigned char *)buffer, pixFmt, width, height);
		av_picture_copy((AVPicture *)internalAVFrame, (AVPicture *)avFrame, pixFmt, width, height);
//		memcpy(internalAVFrame->linesize, avFrame->linesize, 16);
		memcpy(data, internalAVFrame->data, 16);
		memcpy(linesize, internalAVFrame->linesize, 16);
	}
	Frame(unsigned char *buff, PixelFormat pixFmt, unsigned int width, unsigned int height) : pixFmt(pixFmt), width(width), height(height), buffer(buff)
	{
		internalAVFrame = avcodec_alloc_frame();
		avpicture_fill((AVPicture *)internalAVFrame, (unsigned char *)buffer, pixFmt, width, height);
		memcpy(data, internalAVFrame->data, 16);
		memcpy(linesize, internalAVFrame->linesize, 16);
	}
	~Frame()
	{
		free(buffer);
		av_free(internalAVFrame);
	}

	PixelFormat pixFmt;
	unsigned int width, height;
    uint8_t *data[4];
    int linesize[4];

//	AVFrame *operator()(AVFrame *){return internalAVFrame;}
private:
	AVFrame *internalAVFrame;
	unsigned char *buffer;
};

#endif /*FRAME_H*/
