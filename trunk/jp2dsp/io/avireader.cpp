#include <QDir>

#include "avireader.h"

bool aviReader::OpenFile(char *szFolderName)
{
	av_register_all();

	AVFormatContext *ctxt;
	foreach(ctxt, formatContexts) {av_close_input_file(ctxt);}
	//formatContexts.clear();

	QFileInfoList files = QDir::QDir(szFolderName, "*.avi", QDir::Name, QDir::Files).entryInfoList();
	QFileInfo file;
	foreach(file, files)
	{
		StreamType stream = (StreamType)file.baseName().right(1).toInt();
		int res = av_open_input_file(&formatContexts[stream], file.absoluteFilePath().toAscii().constData(), 0, 0, 0);
		res = av_find_stream_info(formatContexts[stream]);
		dump_format(formatContexts[stream], 0, file.absoluteFilePath().toAscii().constData(), 0);
		//break;
		//int h = 15;
	}
	if(formatContexts.isEmpty()){fprintf(stderr, "\naviReader: No *.avi files in folder %s!", szFolderName); return false;}
	totalPlaytime_ms = formatContexts.begin().value()->duration / (AV_TIME_BASE / 1000);
	return true;
}

unsigned int aviReader::GetTotalPlaytime_ms()
{
	if(!formatContexts.isEmpty()) return totalPlaytime_ms;
	return 0;
}

void aviReader::run()
{
	for(QMap<StreamType, AVFormatContext *>::iterator i = formatContexts.begin(); i != formatContexts.end(); ++i) lastTime.insert(i.key(), 0);
	lastSoundTime_ms = 0;
	for(QMap<StreamType, AVFormatContext *>::iterator i = formatContexts.begin();;)
	{
		//seekMutex.lock();
		while(paused)
		{
			isPaused = true;
			msleep(100);
		}
		isPaused = false;
		AVPacket packet;
		AVFormatContext *ctxt = i.value();
		//while(av_read_frame(ctxt, &packet) >= 0)
		if(av_read_frame(ctxt, &packet) < 0)
		{
			av_free_packet(&packet);
			isPaused = true;
			continue;
		}
		
		if(ctxt->streams[packet.stream_index]->codec->codec_type == CODEC_TYPE_AUDIO && decoders->contains(SoundDataID))
		{//≈сли это звуковой пакет
			//av_free_packet(&packet);
			//continue;
			AVRational timeBase = ctxt->streams[packet.stream_index]->time_base;
			//timeBase.num -= 1;
			int time_ms = packet.pts * 1000 * timeBase.num / timeBase.den;
			if(time_ms > lastSoundTime_ms)
			{//“аким образом берЄм звуковой пакет из любого потока, если он новее предыдущего
				unsigned char *buff = new unsigned char[packet.size];
				memcpy(buff, packet.data, packet.size);
				printf("\npacket time %d.               audio %d", time_ms, i.key());
				while(!decoders->value(SoundDataID).decoder->Enqueue(CodedBuff(buff, 0, packet.size, 0, time_ms)))
				{
					if(paused)
					{
						//--currentFrame;
						//if(coded.data1) delete coded.data1;
						//if(coded.data2) delete coded.data2;
						delete buff;
						//av_free_packet(&packet);
						break;
					}
					//msleep(100);
				}
				printf("\npacket time %d.               audio %d enqueued", time_ms, i.key());
				lastSoundTime_ms = time_ms;
			}
		}
		else if(decoders->contains(i.key()))
		{
			AVRational timeBase = ctxt->streams[packet.stream_index]->time_base;
			unsigned char *buff = new unsigned char[packet.size];
			memcpy(buff, packet.data, packet.size);
			int time_ms = packet.pts * 1000 * timeBase.num / timeBase.den;
			printf("\npacket time %d.     video %d", time_ms, i.key());

			while(!decoders->value(i.key()).decoder->Enqueue(CodedBuff(buff, 0, packet.size, 0, time_ms)))
			{
				if(paused)
				{
					//--currentFrame;
					//if(coded.data1) delete coded.data1;
					//if(coded.data2) delete coded.data2;
					delete buff;
					//av_free_packet(&packet);
					break;
				}
				//msleep(100);
			}
			printf("\npacket time %d.     video %d encueued", time_ms, i.key());
			lastTime[i.key()] = time_ms;
		}
		av_free_packet(&packet);
		//“еперь определим, из какого потока последний кадр с самым маленьким временем

		for(QMap<StreamType, int>::iterator lastTimeI = lastTime.begin(); lastTimeI != lastTime.end(); ++lastTimeI)
		{
			if(lastTimeI.value() < lastTime[i.key()]) i = formatContexts.find(lastTimeI.key());
		}
		//seekMutex.unlock();
		//}
	}
}

void aviReader::Seek(unsigned int time_ms)
{
	//seekMutex.lock();

	AVFormatContext *ctxt;
	foreach(ctxt, formatContexts)
	{
		int ret = av_seek_frame(ctxt, -1, time_ms * (AV_TIME_BASE / 1000), 0);
		printf("\nav_seek_frame returnrned %d", ret);
	}
	//foreach(ctxt, formatContexts) av_seek_frame(ctxt, 0, time_ms * 40 / 1000, 0);
	for(QMap<StreamType, int>::iterator i = lastTime.begin() ; i != lastTime.end(); ++i)
	{
		i.value() = 0;
	}
	lastSoundTime_ms = 0;

	//seekMutex.unlock();
}
