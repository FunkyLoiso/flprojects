#include <QLibrary>
#include <QMessageBox>

#include "x264encoder.h"

#include "../commonclasses/common.h"
#include "../frogavcodec/include/ffmpeg/swscale.h"

void X264Encoder::OpenStream(unsigned int frameW, unsigned int frameH, unsigned int bitrate)
{
//	avcodec_init();
//	avcodec_register_all();
	encoder = avcodec_find_encoder(CODEC_ID_H264);
	if (!encoder) {printf("error finding H264 encoder"); return;}
	context = avcodec_alloc_context();
	context->bit_rate = bitrate;
//	context->qmax = 4;
	context->width = frameW;
	context->height = frameH;
	context->time_base.den = 25;
	context->time_base.num = 1;
	context->gop_size = 15;
//	context->pix_fmt = PIX_FMT_RGB32;
	if (avcodec_open(context, encoder) < 0){printf("error opening context");return;}
	frame = avcodec_alloc_frame();
}

int X264Encoder::EncodeFrame(Frame *src, unsigned char *dst, unsigned int &codedSize)
{
//	WHStruct wh;
//	wh.width = src->width;
//	wh.height = src->height;
//	SwsContext *ctxt = sws_getContext(wh.width, wh.height, src->pixFmt, wh.width, wh.height, PIX_FMT_YUV420P, SWS_PARAM_DEFAULT | SWS_FAST_BILINEAR | SWS_CPU_CAPS_MMX | SWS_CPU_CAPS_MMX2, NULL, NULL, NULL);
//	AVFrame *trueSource = avcodec_alloc_frame();
//	unsigned char *trueBuff = (unsigned char *)malloc(avpicture_get_size(PIX_FMT_YUV420P, wh.width, wh.height));
//	avpicture_fill((AVPicture *)trueSource, trueBuff, PIX_FMT_YUV420P, wh.width, wh.height);
//	sws_scale(ctxt, src->data, src->linesize, 0, wh.height, trueSource->data, trueSource->linesize);
//	codedSize = avcodec_encode_video(context, dst, 1600*1200*4, trueSource);
//
//	free(trueBuff);
//	av_free(trueSource);
//	return context->coded_frame->key_frame;

	SwsContext *ctxt = sws_getContext(src->width, src->height, src->pixFmt, src->width, src->height, PIX_FMT_YUV420P, /*SWS_PARAM_DEFAULT | */SWS_FAST_BILINEAR | SWS_CPU_CAPS_MMX | SWS_CPU_CAPS_MMX2, NULL, NULL, NULL);
	AVFrame *trueSource = avcodec_alloc_frame();
	unsigned char *trueBuff = (unsigned char *)malloc(avpicture_get_size(PIX_FMT_YUV420P, src->width, src->height));
	avpicture_fill((AVPicture *)trueSource, trueBuff, PIX_FMT_YUV420P, src->width, src->height);
//	src->linesize[0] = 0;
//	src->linesize[1] = 0;
//	src->linesize[2] = 0;
	sws_scale(ctxt, src->data, src->linesize, 0, src->height, trueSource->data, trueSource->linesize);
	sws_freeContext(ctxt);

	codedSize = avcodec_encode_video(context, dst, /*1600*1200*4*/20000, trueSource);

	free(trueBuff);
	av_free(trueSource);
	return context->coded_frame->key_frame;
}
void X264Encoder::CloseStream()
{
	avcodec_close(context);
	//av_free(decoder);
	av_free(context);
	av_free(frame);
}
