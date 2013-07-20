#include "spoi_to_avi.h"

#include "../ThreadClasses/JP2DecThread.h"
#include "../ThreadClasses/spoisndthread.h"
#include "../commonclasses/frame.h"
#include "../frogavcodec/include/ffmpeg/swscale.h"

SPOI_to_AVI::SPOI_to_AVI() : endReached(false)
{
	av_register_all();

	reader = new SPOIReader(&decoders);
	//fmt = guess_format(formatName, NULL, NULL);

	destFolder = (char *)malloc(256);
	outMapUsed = new QSemaphore();

	for(int i = VideoDataChannel_0; i <= VideoDataChannel_5; ++i)
	{
		outMapFree[(StreamType)i] = new QSemaphore(maxOutMapSizePerStream);
		decoders.insert((StreamType)i, DecoderPair(new JP2DecThread(outMapFree[(StreamType)i], outMapUsed, &outMap, (StreamType)i)));
		QObject::connect(decoders[(StreamType)i].decoder, SIGNAL(BufferReleased(unsigned int)), reader, SLOT(BufferReleased(unsigned int)), Qt::DirectConnection);
		decoders[(StreamType)i].decoder->start();
	}
	outMapFree[SoundDataID] = new QSemaphore(maxOutMapSizePerStream);
	decoders.insert(SoundDataID, DecoderPair(new SPOISndThread(outMapFree[SoundDataID], outMapUsed, &outMap, SoundDataID)));
	QObject::connect(decoders[SoundDataID].decoder, SIGNAL(BufferReleased(unsigned int)), reader, SLOT(BufferReleased(unsigned int)), Qt::DirectConnection);
	decoders[SoundDataID].decoder->start();//TODO: кодирование звука в msv отключено

	QObject::connect(reader, SIGNAL(EndReached()), this, SLOT(EndReached()), Qt::DirectConnection);
}

bool SPOI_to_AVI::OpenFolder(char *name)
{
	if(!reader->OpenFile(name))
	{
		fprintf(stderr, "\nSPOI_to_MSV: folder \"%s\" doesn't contain video data", name);
		return false;
	}
	reader->start();
	return true;
}

bool SPOI_to_AVI::SetDestenation(char *name)
{
	destFolder = (char *)realloc(destFolder, strlen(name) + 1);
	strcpy(destFolder, name);
	return true;
}

//void SPOI_to_AVI::initContext(StreamType stream, int width, int height, int videoBitrate, int channels, int samplerate, int audioBitrate)
//{
//	int res = -18;
//	AVFormatContext *ctxt = formatContexts[stream] = av_alloc_format_context();
//	ctxt->oformat = fmt;
//	sprintf_s(ctxt->filename, sizeof(ctxt->filename), "%s\\stream_%d.%s", destFolder, stream, formatName);
//	AVStream *videoStream = videoStreams[stream] = av_new_stream(ctxt, 0);
//	AVStream *audioStream = audioStreams[stream] = av_new_stream(ctxt, 1);
//
//	AVCodecContext *codecCtxt;
//	AVCodec *codec;
//	//video stream
//	codecCtxt = videoStream->codec;
//	codecCtxt->codec_id = videoCodecID;
//	codecCtxt->codec_type = CODEC_TYPE_VIDEO;
//	codecCtxt->gop_size = 12;
//	codecCtxt->bit_rate = videoBitrate;
//	codecCtxt->width = width;
//	codecCtxt->height = height;
//	codecCtxt->pix_fmt = PIX_FMT_YUV420P;
//	codecCtxt->time_base.num = fpsNum;
//	codecCtxt->time_base.den = fpsDen;
//
//	codec = avcodec_find_encoder(videoCodecID);
//	res = avcodec_open(codecCtxt, codec);
//	//audio stream
//	codecCtxt = audioStream->codec;
//	codecCtxt->codec_id = audioCodecID;
//	codecCtxt->codec_type = CODEC_TYPE_AUDIO;
//	codecCtxt->channels = channels;
//	codecCtxt->sample_rate = samplerate;
//	codecCtxt->bit_rate = audioBitrate;
//
//	codec = avcodec_find_encoder(audioCodecID);
//	res = avcodec_open(codecCtxt, codec);
//
//	res = av_set_parameters(ctxt, NULL);
//	fprintf(stderr, "\n");	
//	dump_format(ctxt, 0, ctxt->filename, 1);
//
//	res = url_fopen(&ctxt->pb, ctxt->filename, URL_WRONLY);
//	res = av_write_header(ctxt);
//}

void SPOI_to_AVI::preread()
{
	//const int prereadTime = 10000;

}

bool SPOI_to_AVI::Convert()
{
	unsigned int totalTime_ms = reader->GetTotalPlaytime_ms();
	while(!endReached)
	{
		if(!outMap.count() && !endReached)
		{
			printf("\nSPOI_to_AVI: outMap empty!");
			Sleep(100);
		}
		int time_ms = outMap.first().first;
		QList<DecodedBuff> timeList(outMap.values(time_ms));
		int rem = outMap.remove(time_ms);
		while(!outMapUsed->tryAcquire(rem, 1000))
		{
			if(endReached) break;
		}
		//if(!outMapUsed->tryAcquire(timeList.count(), 100))
		//{	
		//	//outMapUsed->release();//хмм...)
		//	printf("\nSPOI_to_AVI: cannot ascuire %d sems!", timeList.count());
		//	continue;
		//}
		DecodedBuff buff;
		foreach(buff, timeList)
		{
			outMapFree[buff.stream]->release();

			if(buff.stream == SoundDataID)
			{//Если звук
				AVIEncoder *enc;
				foreach(enc, encoders)
				{
					char *buffer = (char *)malloc(buff.frameSize);
					memcpy(buffer, buff.frame, buff.frameSize);

					enc->Enqueue(time_ms, DecodedBuff(buffer, buff.frameSize, SoundDataID));
				}
				free(buff.frame);
			}
			else if(buff.stream != UndefinedDataID)
			{//Если видео
				if(!encoders.contains(buff.stream))
				{
					Frame *f = (Frame *)buff.frame;
					char fileName[1024] = {0};
					sprintf(fileName, "%s/stream_%d.avi", destFolder, buff.stream);
					encoders[buff.stream] = new AVIEncoder(fileName, f->width, f->height, 350000, 1, 8000, 64000);
					encoders[buff.stream]->start();
				}
				encoders[buff.stream]->Enqueue(time_ms, buff);
			}

			emit Progress(time_ms);
			printf("\nSPOI_to_AVI: convert progress: %0.2f%c (%d ms)", (double)time_ms*100/totalTime_ms, '%', time_ms);
		}
		if(time_ms >= totalTime_ms/*30*1000*/) break;
	}
	printf("\nSPOI_to_AVI: pausing reader..");
	reader->Pause(true);
	while(!reader->isPaused) Sleep(100);
	printf("\nSPOI_to_AVI: reader paused");
	
	DecoderPair decPair;
	foreach(decPair, decoders)
	{
		DecThread *dec = decPair.decoder;
		dec->CleanQueue();
		printf("\nSPOI_to_AVI: decoder %d cleared", dec->stream);
		while(!dec->waitingForData) Sleep(100);
		printf("\nSPOI_to_AVI: decoder %d waiting for data", dec->stream);
	}
	AVIEncoder *enc;
	foreach(enc, encoders)
	{
		enc->Close();
		enc->wait();
	}
	printf("\nSPOI_to_AVI: convert done");
	
	return true;
}

SPOI_to_AVI::~SPOI_to_AVI()
{
	DecoderPair pair;
	foreach(pair, decoders)
	{
		pair.decoder->terminate();
		Sleep(400);
		//while(!pair.decoder->isFinished()) Sleep(100);
		delete outMapFree[pair.decoder->stream];
		delete pair.decoder;
	}
	reader->terminate();
	delete reader;
	delete outMapUsed;
}
