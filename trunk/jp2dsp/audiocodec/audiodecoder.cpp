#include <QMutex>
#include <iostream>

#include "audiodecoder.h"
#include "../commonclasses/common.h"

void AudioDecoder::OpenStream()
{
	static QMutex initMutex;
	QMutexLocker locker(&initMutex);
//	avcodec_init();
//	avcodec_register_all();
	decoder = avcodec_find_decoder(audioCodecID);
	if (!decoder) {printf("error finding audio decoder"); return;}
	context = avcodec_alloc_context();
	context->channels = 1;
	context->sample_rate = 8000;
	if (avcodec_open(context, decoder) < 0){printf("error opening audio decoding context");return;}
}

unsigned int AudioDecoder::DecodeSamples(unsigned char *src, unsigned int srcSize, short *dstSamples)
{
	//printf("\nAACDec: frame size is %d", srcSize);
	int outSize = 0;
	unsigned char *srcPtr = src;
	short *dstPtr = dstSamples;
	//while(srcPtr - src < srcSize)
	//{
	//	outSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;
	//	int size = avcodec_decode_audio2(context, dstPtr, &outSize, srcPtr+2, *((short *)srcPtr));
	//	srcPtr += (size + 2);
	//	dstPtr += /*context->frame_size * context->channels*/outSize/2;
	//}
	//return /*(dstPtr - dstSamples)*sizeof(short)*/16000;
	outSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;
	int size = avcodec_decode_audio2(context, dstPtr, &outSize, srcPtr, srcSize);
	return outSize;
}

void AudioDecoder::CloseStream()
{
	avcodec_close(context);
	av_free(context);
}
