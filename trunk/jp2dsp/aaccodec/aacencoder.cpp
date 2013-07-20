#include <QMutex>
#include <iostream>

#include "aacencoder.h"

void AACEncoder::OpenStream(unsigned int nChannels,unsigned int sampleRate)
{
//	static QMutex initMutex;
//	QMutexLocker locker(&initMutex);
//	avcodec_init();
//	avcodec_register_all();
	encoder = avcodec_find_encoder(CODEC_ID_MP3LAME);
	if (!encoder) {printf("error finding AAC encoder"); return;}
	context = avcodec_alloc_context();
	context->channels = nChannels;
	context->sample_rate = sampleRate;
	context->bit_rate = 24000;//μμ..?
//	context->frame_size = 2000;
	if (avcodec_open(context, encoder) < 0){printf("error opening AAC encoder context");return;}
	
}

unsigned int AACEncoder::EncodeSamples(short *srcSamples, unsigned int nSamples, unsigned char *dst)
{
//	context->frame_size = /*nSamples*/1024;
	unsigned int n = avcodec_encode_audio(context, dst, 0xffff, srcSamples);
	return n;
}

void AACEncoder::CloseStream()
{
	avcodec_close(context);
	av_free(context);
}
