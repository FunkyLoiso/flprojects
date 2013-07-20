#include <QMutex>
#include <iostream>

#include "aacdecoder.h"

void AACDecoder::OpenStream()
{
	static QMutex initMutex;
	QMutexLocker locker(&initMutex);
//	avcodec_init();
//	avcodec_register_all();
	decoder = avcodec_find_decoder(CODEC_ID_MP3LAME);
	if (!decoder) {printf("error finding AAC decoder"); return;}
	context = avcodec_alloc_context();
	context->channels = 2;
	context->sample_rate = 48000;
	if (avcodec_open(context, decoder) < 0){printf("error opening AAC decoding context");return;}
}

unsigned int AACDecoder::DecodeSamples(unsigned char *src, unsigned int srcSize, short *dstSamples)
{
	//printf("\nAACDec: frame size is %d", srcSize);
	int outSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;
	avcodec_decode_audio2(context, dstSamples, &outSize, src, srcSize);
	return outSize; 
}

void AACDecoder::CloseStream()
{
	avcodec_close(context);
	av_free(context);
}
