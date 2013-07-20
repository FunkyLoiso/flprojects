#include <QMutex>
#include <iostream>

#include "audioencoder.h"

void AudioEncoder::OpenStream(unsigned int nChannels,unsigned int sampleRate)
{
//	static QMutex initMutex;
//	QMutexLocker locker(&initMutex);
//	avcodec_init();
//	avcodec_register_all();
	encoder = avcodec_find_encoder(CODEC_ID_AC3);
	if (!encoder) {printf("error finding audio encoder"); return;}
	context = avcodec_alloc_context();
	context->channels = nChannels;
	context->sample_rate = sampleRate;
	context->bit_rate = 24000;//μμ..?
//	context->frame_size = 2000;
	if (avcodec_open(context, encoder) < 0){printf("error opening audio encoder context");return;}
	
}

unsigned int AudioEncoder::EncodeSamples(short *srcSamples, unsigned int nSamples, unsigned char *dst)
{
	//context->frame_size = /*nSamples*/1024;
	short *srcPtr = srcSamples;
	unsigned char *dstPtr = dst;
	while(srcPtr - srcSamples < (nSamples - (context->frame_size * context->channels)))
	{
		int size = avcodec_encode_audio(context, dstPtr + 2, 16000, srcPtr);
		if(size != 0)
		{
			*((short*)dstPtr) = size;
			dstPtr += (size + 2);
		}
		srcPtr += context->frame_size * context->channels;
	}
	return dstPtr - dst;
}

void AudioEncoder::CloseStream()
{
	avcodec_close(context);
	av_free(context);
}
