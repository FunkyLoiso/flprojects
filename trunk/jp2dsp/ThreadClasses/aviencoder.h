#ifndef AVIENCODER_H
#define AVIENCODER_H

#include <QThread>

extern "C"
{
#include "../frogavcodec/include/ffmpeg/avformat.h"
}
#include "../frogavcodec/include/ffmpeg/swscale.h"

#include "../commonclasses/common.h"
#include "../commonclasses/frame.h"

class AVIEncoder : public QThread
{
	Q_OBJECT
public:
	AVIEncoder(char *fileName, int width, int height, int videoBitrate, int channels, int samplerate, int audioBitrate);
	bool Enqueue(int time_ms, DecodedBuff buff);
	void Close();
private:
	void run();

	static const int fpsNum = 1;
	static const int fpsDen = 25;

	AVOutputFormat *fmt;
	AVFormatContext *ctxt;
	AVStream *videoStream, *audioStream;
	QMap<int, DecodedBuff> inMap;
	QMutex mapMutex;
	QSemaphore mapSem;
	bool waitingForData;
	bool close;

	int restSize;
	unsigned char *temp;
};

#endif