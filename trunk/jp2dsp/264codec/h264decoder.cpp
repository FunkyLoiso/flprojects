#include <QLibrary>
//#include <QMessageBox>
#include <QMutex>
extern "C"
{
#include "h264decoder.h"
}

#include <windows.h>
#include "../commonclasses/common.h"
#include "../commonclasses/frame.h"

void H264Decoder::OpenStream()
{
	static QMutex initMutex;
	QMutexLocker locker(&initMutex);
	decoder = avcodec_find_decoder(videoCodecID);
	//decoder = avcodec_find_decoder(CODEC_ID_MPEG4);
	if (!decoder) {printf("error finding video decoder"); return;}
	context = avcodec_alloc_context();
	//context->workaround_bugs = 1;
	//context->debug = FF_DEBUG_PICT_INFO;
	if (avcodec_open(context, decoder) < 0){printf("error opening context");return;}
	frame = avcodec_alloc_frame();
}

Frame *H264Decoder::DecodeFrame(unsigned char *src, unsigned int srcSize)
{
	if(context == 0 || src == 0) return 0;
	int frameReady;

	//static bool once = false;
	//static unsigned char trulo[2000000];
	//static int firstSize = srcSize;
	//if(!once)
	//{
	//	memcpy(trulo, src, srcSize);
	//	once = true;
	//}
	//int decoded, remaining = firstSize;
	//unsigned char *tmp = trulo;

	int decoded, remaining = srcSize;
	unsigned char *tmp = src;
	
	do
	{
		printf("\nH264Decoder: before decode. data is %x%x%x%x%x%x%x%x size is %d", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7], srcSize);
		decoded = avcodec_decode_video(context, frame, &frameReady, tmp, remaining);
		printf("\nH264Decoder: decoded = %d", decoded);
		if(decoded <= 0) return false;
		remaining -= decoded;
		tmp += decoded;
	}while(!frameReady && remaining);
	printf("\nH264Decoder: done decoding");
//	AVFrame *pic = avcodec_alloc_frame();
//	uint8_t *buffer = (uint8_t *)malloc(avpicture_get_size(context->pix_fmt, context->width, context->height));
//	avpicture_fill((AVPicture *)pic, buffer, context->pix_fmt, context->width, context->height);
//	av_picture_copy((AVPicture *)pic, (AVPicture *)frame, context->pix_fmt, context->width, context->height);

//	WHStruct *wh = (WHStruct *)&(pic->opaque);
//	wh->width = context->width;
//	wh->height = context->height;
	Frame *f = new Frame(frame, context->pix_fmt, context->width, context->height);
	printf("\nH264Decoder: created frame, returning it");
	return f;

//	return pic;//Похоже, кадр будет в YUV420P
}

void H264Decoder::GetImageDims(unsigned int &width, unsigned int &height)
{
	width = context->width;
	height = context->height;
}

void H264Decoder::CloseStream()
{
	avcodec_close(context);
	//av_free(decoder);
	av_free(context);
	av_free(frame);
	context = 0;
	decoder = 0;
	frame = 0;
}
