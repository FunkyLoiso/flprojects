#include <QLibrary>

#include "spoireader.h"

#define RevBytes(val)(\
		((val & 0xff000000) >> 24) + \
		((val & 0x00ff0000) >> 8 ) + \
		((val & 0x0000ff00) << 8 ) + \
		((val & 0x000000ff) << 24))

SPOIReader::SPOIReader(QMap<StreamType, DecoderPair> *decoders) : IFileReader(decoders), currentFrame(0), paused(false)
{
	QLibrary lib;
	lib.setFileName("SPOI_Access");
	if(!lib.load())
	{
		fprintf(stderr, "Cannot load SPOI_Access.dll!");
		exit(-118);
	}
	SPOI_Init = (TSPOI_Init)lib.resolve("SPOI_Init");
	SPOI_Free = (TSPOI_Free)lib.resolve("SPOI_Free");
	SPOI_openFile = (TSPOI_openFile)lib.resolve( "SPOI_openFile");
	SPOI_openDevice = (TSPOI_openDevice)lib.resolve("SPOI_openDevice");
	SPOI_closeDataSource = (TSPOI_closeDataSource)lib.resolve("SPOI_closeDataSource");
	SPOI_getInfo = (TSPOI_getInfo)lib.resolve("SPOI_getInfo");
	SPOI_getFrame = (TSPOI_getFrame)lib.resolve("SPOI_getFrame");
	SPOI_findFrame = (TSPOI_findFrame)lib.resolve("SPOI_findFrame");

	SPOI_Init();
}

SPOIReader::~SPOIReader()
{
	SPOI_Free();
}

bool SPOIReader::OpenFile(char *szFileName)
{
	currentFrame = 0;
	if (szFileName)
	{//≈сли им€ - не нулевой указатель, то открываем папу, иначе - устройство
		if(!SPOI_openFile(szFileName))
		{
			fprintf(stderr, "\nSPOIReader: wrong folder name: %s", szFileName);
			return false;
		}
	}
	else
	{
		if(!SPOI_openDevice())
		{
			fprintf(stderr, "\nSPOIReader: no folder given, and no SPOI device found");
			return false;
		}
	}
	SPOI_getInfo(fileInfo);

	return true;
}

void SPOIReader::Seek(unsigned int time_ms)
{
	
	if(time_ms > fileInfo.playbackTime * tickSize_ms)
	{
		fprintf(stderr, "\nSPOIReader: cannot seek time %d, max time is %d", time_ms, fileInfo.playbackTime * tickSize_ms);
		return;
	}
	seekMutex.lock();
	currentFrame = SPOI_findFrame(time_ms / tickSize_ms);
	seekMutex.unlock();
	printf("\nSPOIReader: seek to %d done", time_ms);
}

void SPOIReader::run()
{
	static SFrame frame;
	static unsigned long *buff1, *buff2;
	//static unsigned char *curBufferPos;
	//static CodedBuff coded;

	for(;;)
	{
		if(currentFrame >= fileInfo.nFrame)
		{
			printf("\nSPOIReader: EOF reached");
			emit EndReached();
			paused = true;
		}
		while(paused)
		{
			isPaused = true;
			msleep(100);
		}
		isPaused = false;
		SPOI_getFrame(++currentFrame, frame);
		if(!decoders->contains(frame.id)) {/*seekMutex.unlock(), pauseMutex.unlock(); */continue;};

		CodedBuff coded;
		if(frame.id != SoundDataID)
		//if(frame.id == VideoDataChannel_0 && frame.id != SoundDataID)
		{//ƒл€ видеокадров
			buff1 = (unsigned long *)frame.frameData;

			if(RevBytes(buff1[4]) != 7 || RevBytes(buff1[16]) != 7) {/*seekMutex.unlock(), pauseMutex.unlock(); */continue;};//ѕроверка целостности
			coded.size1 = RevBytes(buff1[3]) + RevBytes(buff1[15]);
			buff2 = buff1 + 24 + coded.size1;
			if(RevBytes(buff2[4]) != 7 || RevBytes(buff2[16]) != 7) {/*seekMutex.unlock(), pauseMutex.unlock(); */continue;};
			coded.size2 = RevBytes(buff2[3]) + RevBytes(buff2[15]);

			coded.size1 *= 4, coded.size2 *= 4;//“ак как размер в DWORD-ах
			//memcpy(curBufferPos, buff1+24, coded.size1);
			coded.data1 = new unsigned char[coded.size1];
			memcpy(coded.data1, buff1+24, coded.size1);
			//coded.data1 = curBufferPos;
			//memcpy(curBufferPos += coded.size1, buff2+24, coded.size2);//«десь сдвигаетс€ curBufferPos
			coded.data2 = new unsigned char[coded.size2];
			memcpy(coded.data2, buff2+24, coded.size2);
			//coded.data2 = curBufferPos;
			//curBufferPos += coded.size2;//—двигаем указатель после того, как записали вторую часть кадра
			coded.time_ms = frame.time * tickSize_ms;
		}
		//else {seekMutex.unlock(); pauseMutex.unlock(); continue;}
		else
		{//дл€ аудиокадров
			//memcpy(curBufferPos, frame.frameData, frame.size);
			coded.data1 = new unsigned char[frame.size/* + 4000*/];
			//memset(coded.data1 + frame.size, 0, 4000);
			memcpy(coded.data1, frame.frameData, frame.size);
			//coded.data1 = curBufferPos;
			coded.size1 = frame.size;
			coded.time_ms = frame.time * tickSize_ms;

			//curBufferPos += frame.size;//—двигаем указатель после того, как записали по нему кадр
		}

		//«десь будет распределение по потокам
		//printf("\nSPOIReader: before enqueue %d for stream %d", frame.time * tickSize_ms, frame.id);
		while(!decoders->value(frame.id).decoder->Enqueue(coded))
		{//≈сли очередь декодера полна
			
			if(paused)
			{
				--currentFrame;
				if(coded.data1) delete coded.data1;
				if(coded.data2) delete coded.data2;
				break;
			}
		}
		//seekMutex.unlock();
		//pauseMutex.unlock();
		//printf("\nSPOIReader: done   enqueue %d for stream %d", frame.time * tickSize_ms, frame.id);
	}
	
}
