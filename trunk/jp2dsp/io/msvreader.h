#ifndef MSVREADER_H
#define MSVREADER_H

#include <QThread>
#include <QSemaphore>
#include <QFile>
#include <QMutex>
#include <QMap>
#include "ifilereader.h"

class msvReader : public IFileReader
{
public:
	msvReader(QMap<StreamType, DecoderPair> *renderers) : IFileReader(renderers){};
	bool OpenFile(char *szFileName);
	void Seek(unsigned int time_ms);
	unsigned int GetTotalPlaytime_ms();
	void Pause(bool pause){pause ? pauseMutex.lock() : pauseMutex.unlock();};

private:
	void buildIndex();
	void run();

	QFile file;
	QList<IndexPair> fileIndexes;
	QMutex pauseMutex, seekMutex;

	static const int bufferSize = 1048576 * 5;
	static const int framesPerIndex = 100;
	unsigned char buffer[bufferSize], *buffPos;
	msvFrameHeader *header;
	unsigned int totalRead;

	unsigned int totalPlaytime_ms;
};

#endif /*MSVREADER_H*/
