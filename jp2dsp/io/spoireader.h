#ifndef SPOIREADER_H
#define SPOIREADER_H

#include <QMap>
#include <QThread>

#include "ifilereader.h"
#include "SpoiInterface35.h"

extern "C"
{
typedef void (* TSPOI_Init) ();
typedef void (* TSPOI_Free) ();
typedef bool (* TSPOI_openFile) (const char *);
typedef bool (* TSPOI_openDevice) ();
typedef void (* TSPOI_closeDataSource) ();
typedef bool (* TSPOI_getInfo) (SSummaryInfo &);
typedef bool (* TSPOI_getFrame) (unsigned int, SFrame &);
typedef int (* TSPOI_findFrame) (unsigned int);
}

class SPOIReader : public IFileReader
{
	Q_OBJECT
public:
	SPOIReader(QMap<StreamType, DecoderPair> *decoders);
	~SPOIReader();
	bool OpenFile(char * szFileName);
	void Seek(unsigned int time_ms);
	unsigned int GetTotalPlaytime_ms(){return fileInfo.playbackTime * tickSize_ms;};
	void Pause(bool pause) {paused = pause;};

signals:
	void EndReached();

private:
	void run();

	SSummaryInfo fileInfo;
	unsigned int currentFrame;
	QMutex seekMutex, pauseMutex;
	static const int tickSize_ms = 1000/64;
	bool paused;

	TSPOI_Init SPOI_Init;
	TSPOI_Free SPOI_Free;
	TSPOI_openFile SPOI_openFile;
	TSPOI_openDevice SPOI_openDevice;
	TSPOI_closeDataSource SPOI_closeDataSource;
	TSPOI_getInfo SPOI_getInfo;
	TSPOI_getFrame SPOI_getFrame;
	TSPOI_findFrame SPOI_findFrame;
};

#endif /*SPOIREADER_H*/
