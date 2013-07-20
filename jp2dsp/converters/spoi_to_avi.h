#ifndef SPOI_TO_AVI_H
#define SPOI_TO_AVI_H

#include <QMap>
#include <QSemaphore>
//extern "C"
//{
//#include "../frogavcodec/include/ffmpeg/avformat.h"
//}
#include "../commonclasses/qmultimapts.h"

#include "../io/spoireader.h"
#include "../io/msvwriter.h"
#include "../264codec/x264encoder.h"
#include "../audiocodec/audioencoder.h"
#include "../ThreadClasses/aviencoder.h"

class SPOI_to_AVI : public QObject
{
	Q_OBJECT
public:
	SPOI_to_AVI();
	~SPOI_to_AVI();
	bool OpenFolder(char *name);
	bool SetDestenation(char *name);
	bool Convert();
	int getMaxTime_ms(){return reader->GetTotalPlaytime_ms();};

signals:
	void Progress(int time_ms);

private:
	QMap<StreamType, DecoderPair> decoders;
	QMap<StreamType, AVIEncoder *> encoders;

	SPOIReader *reader;

	char *destFolder;
	QSemaphore *outMapFree[NumDataTypes], *outMapUsed;
	QMultiMapTS<int, DecodedBuff> outMap;
	bool endReached;

	char *debugSoundSource;

	void preread();//Тестовое чтение файла, чтобы определить, какие в нём есть потоки
	//void initContext(StreamType stream, int width, int height, int videoBitrate, int channels, int samplerate, int audioBitrate);//Инициализировать контекст и видеострим

private slots:
	void EndReached()
	{
		endReached = true;
		printf("\nSPOI_to_AVI: endReached is true");
	};
};

#endif /* SPOI_TO_AVI_H */
