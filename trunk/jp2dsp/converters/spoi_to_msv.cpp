#include "spoi_to_msv.h"

#include "../ThreadClasses/JP2DecThread.h"
#include "../ThreadClasses/spoisndthread.h"
#include "../commonclasses/frame.h"

SPOI_to_MSV::SPOI_to_MSV() : audioEncoder(0), endReached(false)
{
	reader = new SPOIReader(&decoders);
	for(int i = VideoDataChannel_3; i <= VideoDataChannel_3; ++i)
	{
		outMapFree[(StreamType)i] = new QSemaphore(maxOutMapSizePerStream);
		decoders.insert((StreamType)i, DecoderPair(new JP2DecThread(outMapFree[(StreamType)i], &outMapUsed, &outMap, (StreamType)i)));
		QObject::connect(decoders[(StreamType)i].decoder, SIGNAL(BufferReleased(unsigned int)), reader, SLOT(BufferReleased(unsigned int)), Qt::DirectConnection);
		decoders[(StreamType)i].decoder->start();
	}
	outMapFree[SoundDataID] = new QSemaphore(maxOutMapSizePerStream);
	decoders.insert(SoundDataID, DecoderPair(new SPOISndThread(outMapFree[SoundDataID], &outMapUsed, &outMap, SoundDataID)));
	QObject::connect(decoders[SoundDataID].decoder, SIGNAL(BufferReleased(unsigned int)), reader, SLOT(BufferReleased(unsigned int)), Qt::DirectConnection);
	decoders[SoundDataID].decoder->start();//TODO: кодирование звука в msv отключено
//	outMapFree[VideoDataChannel_2] = new QSemaphore(maxOutMapSizePerStream);
//	decoders.insert(VideoDataChannel_2, DecoderPair(new JP2DecThread(outMapFree[VideoDataChannel_2], &outMapUsed, &outMap, VideoDataChannel_2)));
//	QObject::connect(decoders[VideoDataChannel_2].decoder, SIGNAL(BufferReleased(unsigned int)), reader, SLOT(BufferReleased(unsigned int)), Qt::DirectConnection);
//	decoders[VideoDataChannel_2].decoder->start();

	QObject::connect(reader, SIGNAL(EndReached()), this, SLOT(EndReached()), Qt::DirectConnection);
}

bool SPOI_to_MSV::OpenFolder(char *name)
{
	if(!reader->OpenFile(name))
	{
		fprintf(stderr, "\nSPOI_to_MSV: folder \"%s\" doesn't contain video data", name);
		return false;
	}
	reader->start();
	return true;
}

bool SPOI_to_MSV::SetDestenation(char *name)
{
	if(!writer.OpenFile(name))
	{
		fprintf(stderr, "\nSPOI_to_MSV: cannot open destenation file \"%s\"", name);
		return false;
	}
	return true;
}

bool SPOI_to_MSV::Convert()
{
	unsigned int totalTime_ms = reader->GetTotalPlaytime_ms();
	unsigned int size;
//	unsigned char temp[1408*1050*4];
	unsigned char *temp = (unsigned char *)malloc(1600 * 1200 * 4);
	while(true/*outMap.count() != 0*/)
	{
//		Sleep(500);
		//printf("\nSPOI_to_MSV: outMapUsed.available() is %d", outMapUsed.available());
		if(!endReached) outMapUsed.acquire();
		else
		{
			if(!outMapUsed.tryAcquire(1, 1000))
			{
				break;
			}
		}
		int time_ms = outMap.first().first;
		QList<DecodedBuff> timeList = outMap.values(time_ms);
		if(!endReached) outMapUsed.acquire(outMap.remove(time_ms) - 1);
		else
		{
			if(!outMapUsed.tryAcquire(outMap.remove(time_ms) - 1, 1000))
			{
				break;
			}
		}
		DecodedBuff buff;
		foreach(buff, timeList)
		{
			//printf("\noutMapFree[%d].availabe() is %d, releasing..", buff.stream, outMapFree[buff.stream]->available());
			outMapFree[buff.stream]->release();
			//printf("\noutMap now contains: %d. Got frame ptr: %d, stream: %d", outMap.count(), buff.frame, buff.stream);

			if(buff.stream == SoundDataID)
			{//Если звук
				if(!audioEncoder)
				{//Инициализация энкодера звука
					audioEncoder = new AudioEncoder();
					audioEncoder->OpenStream(1, 8000);
				}

				size = audioEncoder->EncodeSamples((short *)buff.frame, buff.frameSize/2, temp);
				free(buff.frame);
			}
			else
			{//Если видео
				Frame *f = (Frame *)buff.frame;
				if(!videoEncoders.contains(buff.stream))
				{//инициализация энкодера видео
					videoEncoders.insert(buff.stream, new X264Encoder());
					videoEncoders[buff.stream]->OpenStream(f->width, f->height, 0);
				}
				videoEncoders[buff.stream]->EncodeFrame(f, temp, size);
				delete f;
			}
			writer.WriteFrame(temp, size, time_ms, buff.stream, 0);
			emit Progress(time_ms);
			printf("\nSPOI_to_MSV: convert progress: %d%c", time_ms*100/totalTime_ms, '%');
		}
		if(time_ms >= /*totalTime_ms*/60*1000) break;
	}
	writer.CloseFile();
	free(temp);
	printf("\nSPOI_to_MSV: convert done");
	return true;
}
