#ifndef AVIREADER_H
#define AVIREADER_H

#include <QMap>
#include <QFile>
extern "C"
{
#include "../frogavcodec/include/ffmpeg/avformat.h"
}

#include "SpoiInterface35.h"
#include "ifilereader.h"

class aviReader : public IFileReader
{
public:
	aviReader(QMap<StreamType, DecoderPair> *renderers) : IFileReader(renderers), paused(false){};
	bool OpenFile(char *szFileName);
	void Seek(unsigned int time_ms);
	unsigned int GetTotalPlaytime_ms();
	void Pause(bool pause){paused = pause;};

private:
	void buildIndex();
	void run();

	bool paused;

	QMap<StreamType, AVFormatContext*> formatContexts;
	//QList<IndexPair> fileIndexes;
	QMutex pauseMutex, seekMutex;

	QMap<StreamType, int> lastTime;
	int lastSoundTime_ms;

	//static const int bufferSize = 1048576 * 5;
	//static const int framesPerIndex = 100;
	//unsigned char buffer[bufferSize], *buffPos;
	msvFrameHeader *header;
	unsigned int totalRead;

	unsigned int totalPlaytime_ms;
};

#endif